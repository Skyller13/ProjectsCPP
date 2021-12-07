// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootAbilityBase.h"


#include "ProjectileTemplate.h"
#include "AbilityActor/Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectKapy/CatCharacterMovement.h"
#include "ProjectKapy/ProjectKapyCharacter.h"
#include "ProjectKapy/Gameplay/Actors/CastFX.h"
#include "ProjectKapy/Gameplay/Combat/EffectReceiverComponent.h"
#include "ProjectKapy/Gameplay/Combat/ManaSystemComponent.h"

UShootAbilityBase::UShootAbilityBase()
{
	ActionName = "Fire";
}

void UShootAbilityBase::Init(UAbilityManagerComponent* AbilityMng)
{
	Super::Init(AbilityMng);
	ManaSystem = AbilityMng->GetOwner()->FindComponentByClass<UManaSystemComponent>();
	if (CastFX)
	{
		CastFXInstanced = AbilityManager->GetWorld()->SpawnActor<ACastFX>(CastFX);
		if (CastFXInstanced)
		{
			CastFXInstanced->AttachToComponent(Character->GetMesh(),
			                                   FAttachmentTransformRules::SnapToTargetIncludingScale, Socket);
			CastFXInstanced->SetShootAbility(this);
		}
	}

	SetMainProjectileTemplate(ProjectileTemplateSubclass);
	CurrentState = FS_None;
}

void UShootAbilityBase::Destroy()
{
	Super::Destroy();
	if (CastFXInstanced && !CastFXInstanced->IsPendingKill())
	{
		CastFXInstanced->Destroy();
	}
}

void UShootAbilityBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurrentState)
	{
	case FS_None:
		if (bIsPress && IsExecutionValid())
		{
			Execute();
		}
		break;
	case FS_Cast:
		CastTimer += DeltaTime;
		if (CastTimer >= ProjectileTemplate->CastTime)
		{
			CurrentState = FS_Firing;
			PlayFiringAnim(ProjectileTemplate);
			TimeToLaunchProjectile = 0.f;
			ShootTimer = ProjectileTemplate->NbShootByMagazine;
		}
		break;
	case FS_Firing:
		TimeToLaunchProjectile += DeltaTime;
		if (TimeToLaunchProjectile >= TimerToLaunchProjectile)
		{
			ShootTimer += DeltaTime;
			if (ShootTimer > ProjectileTemplate->TimeByShoot)
			{
				ShootTimer = 0.f;
				LaunchProjectile(ProjectileTemplate);
				ShootCount++;
				if (ShootCount >= ProjectileTemplate->NbShootByMagazine)
				{
					if (Character->CharacterState == ECharacterState::STATE_Shoot)
					{
						Character->SetCharacterState(ECharacterState::STATE_None);
					}
					CurrentState = FS_Recovery;
					CastTimer = 0.f;
					ShootCount = 0;
					if (ManaSystem != nullptr)
						ManaSystem->ConsumeMana(ProjectileTemplate->ManaCost);

					bIsExecuting = false;
					RecoveryTimer = ProjectileTemplate->RecoveryTime;
				}
			}
		}
		break;
	case FS_Recovery:
		RecoveryTimer -= DeltaTime;
		if (RecoveryTimer <= 0.f)
		{
			CurrentState = FS_None;
			PressElapsedTime = 0.f;
			ExecutingElapsedTime = 0.f;
		}
		break;
	default:
		break;
	}
}


void UShootAbilityBase::Interrupt()
{
	if (CurrentState == FS_Firing)
	{
		if (ProjectileTemplate && ProjectileTemplate->FiringAnim)
		{
			Character->StopAnimMontage(ProjectileTemplate->FiringAnim);
		}
		CurrentState = FS_Recovery;
		RecoveryTimer = ProjectileTemplate->RecoveryTime;
	}
	else
	{
		CurrentState = FS_None;
	}

	if (Character->CharacterState == ECharacterState::STATE_Shoot)
	{
		Character->SetCharacterState(ECharacterState::STATE_None);
	}

	CastTimer = 0.f;
	ShootCount = 0;

	Super::Interrupt();
}

void UShootAbilityBase::Execute()
{
	Super::Execute();

	if (Character)
	{
		Character->SetCharacterState(STATE_Shoot);
	}

	CastTimer = 0.f;
	CurrentState = FS_Cast;
}

bool UShootAbilityBase::IsExecutionValid() const
{
	if (CurrentState != FS_None)
	{
		return false;
	}

	if (Character)
	{
		if (Character->IsPlayerControlled() && !Character->isAimingMode)
		{
			return false;
		}
	}

	if (ProjectileTemplate && ManaSystem)
	{
		if (!ManaSystem->HaveEnoughtMana(ProjectileTemplate->ManaCost))
		{
			return false;
		}
	}

	return Super::IsExecutionValid();
}

void UShootAbilityBase::AbilityPressed()
{
	Super::AbilityPressed();
	if (IsExecutionValid())
	{
		Execute();
	}
}

void UShootAbilityBase::OnCharacterReceiveDamage(AActor* DamagedActor, float Value, AActor* DamageCauser)
{
	Super::OnCharacterReceiveDamage(DamagedActor, Value, DamageCauser);
	if (bDamageSensitive)
	{
		Interrupt();
	}
}

bool UShootAbilityBase::CanShoot()
{
	if (Character)
	{
		if (Character->IsPlayerControlled() && !Character->isAimingMode)
		{
			return false;
		}

		if (ProjectileTemplate && ManaSystem)
		{
			if (!ManaSystem->HaveEnoughtMana(ProjectileTemplate->ManaCost))
			{
				return false;
			}
		}

		if (CurrentState == FS_Recovery)
		{
			return false;
		}
	}
	return true;
}

void UShootAbilityBase::SetAimDirection(FVector Direction)
{
	AimDirection = Direction.GetSafeNormal();
}

void UShootAbilityBase::LaunchProjectile(UProjectileTemplate* Projectile)
{
	if (Projectile == nullptr)
		return;

	OnShootBegin.Broadcast(Projectile);

	FTransform Transform;
	if (!Socket.IsNone())
	{
		Transform = Character->GetMesh()->GetSocketTransform(Socket);
	}
	else
	{
		Transform = Character->GetActorTransform();
	}

	FVector AimDir = AimDirection + Projectile->AimOffset;
	AimDir.Normalize();
	float angleStart = -Projectile->ConeSizeDegrees / 2.f;
	float anglePart = Projectile->ConeSizeDegrees;
	const FVector aimUp = AimDir.ToOrientationQuat().GetUpVector();

	if (Projectile->NbProjectileByShoot > 1)
	{
		anglePart /= (Projectile->NbProjectileByShoot - 1);
	}

	for (int i = 0; i < Projectile->NbProjectileByShoot; ++i)
	{
		AProjectile* ProjectileInstancied = InstanciateProjectile(Projectile, Transform);
		float angle = angleStart + anglePart * i;
		FVector direction = AimDir;
		direction = direction.RotateAngleAxis(angle, aimUp);
		ProjectileInstancied->SetupProjectile(direction);
	}
}

void UShootAbilityBase::PlayFiringAnim(UProjectileTemplate* Projectile)
{
	if (Projectile && Projectile->FiringAnim != nullptr)
	{
		for (FAnimNotifyEvent Notify : Projectile->FiringAnim->Notifies)
		{
			if (Notify.NotifyName == "LaunchProjectile")
			{
				TimerToLaunchProjectile = Notify.GetTime() / Projectile->AnimRate;
			}
		}

		if (Character)
			Character->PlayAnimMontage(Projectile->FiringAnim, Projectile->AnimRate);
	}
	else
	{
		TimerToLaunchProjectile = 0.f;
	}
}

AProjectile* UShootAbilityBase::InstanciateProjectile(UProjectileTemplate* Projectile, FTransform Transform)
{
	if (Projectile)
	{
		AProjectile* ProjectileInstancied = AbilityManager->GetWorld()->SpawnActorDeferred<AProjectile>(
			Projectile->ProjectileActor, Transform, Character, Character);
		ProjectileInstancied->Init(this);
		ProjectileInstancied->OverrideProperties(Projectile->ComputedProperties);
		ProjectileInstancied->FinishSpawning(Transform);
		return ProjectileInstancied;
	}
	return nullptr;
}

void UShootAbilityBase::AimAt(FVector TargetLocation)
{
	FVector outLaunchVelocity;
	FVector startLocation = Character->GetActorLocation();
	if (!Socket.IsNone())
	{
		startLocation = Character->GetMesh()->GetSocketLocation(Socket);
	}

	if (!ProjectileTemplate->ComputedProperties.bUseGravity)
	{
		SetAimDirection(TargetLocation - startLocation);
	}
	else
	{
		bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity
		(
			this,
			outLaunchVelocity,
			startLocation,
			TargetLocation,
			ProjectileTemplate->ComputedProperties.LaunchSpeed,
			false,
			0.f,
			0.f,
			ESuggestProjVelocityTraceOption::DoNotTrace
		);

		if (bHaveAimSolution) // Calculate the out launch velocity	
		{
			SetAimDirection(outLaunchVelocity);
		}
	}
}

bool UShootAbilityBase::CanFireProjectile(UProjectileTemplate* Projectile)
{
	if (Projectile && Projectile->CastTime < CastTimer)
	{
		if (ManaSystem)
		{
			if (ManaSystem->HaveEnoughtMana(Projectile->ManaCost))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

float UShootAbilityBase::GetChargePercent() const
{
	if (ProjectileTemplate && ProjectileTemplate->CastTime != 0.f)
	{
		return FMath::Clamp(CastTimer / ProjectileTemplate->CastTime, 0.f, 1.f);
	}
	return 0.f;
}


float UShootAbilityBase::GetTimeBeforeShootPercent() const
{
	if (TimerToLaunchProjectile != 0.f)
	{
		return FMath::Clamp(TimeToLaunchProjectile / TimerToLaunchProjectile, 0.f, 1.f);
	}
	return 0.f;
}

UProjectileTemplate* UShootAbilityBase::GetMainProjectileTemplate() const
{
	return ProjectileTemplate;
}

void UShootAbilityBase::SetMainProjectileTemplate(TSubclassOf<UProjectileTemplate> MainProjectileTemplate)
{
	if (MainProjectileTemplate)
	{
		if (ProjectileTemplate)
		{
			ProjectileTemplate->ConditionalBeginDestroy();
			ProjectileTemplate = nullptr;
		}

		ProjectileTemplate = NewObject<UProjectileTemplate>(Character, MainProjectileTemplate);
		if (ProjectileTemplate)
			ProjectileTemplate->ComputeProperties();
	}
}
