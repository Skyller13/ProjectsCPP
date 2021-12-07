// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootAbility.h"

#include "ProjectileTemplate.h"
#include "PhysicsEngine/PhysicsSettings.h"
#include "ProjectKapy/ProjectKapyCharacter.h"
#include "ProjectKapy/Gameplay/Combat/ManaSystemComponent.h"

UShootAbility::UShootAbility()
{
	ActionName = "Fire";
	CurrentState = FS_None;
}

void UShootAbility::Init(UAbilityManagerComponent* AbilityMng)
{
	Super::Init(AbilityMng);
	SetHeavyProjectileTemplate(HeavyProjectileSubclass);
}

void UShootAbility::Tick(float DeltaTime)
{
	UAbilityBase::Tick(DeltaTime);

	switch (CurrentState)
	{
	case FS_None:
		if (bIsPress)
		{
			CurrentState = FS_Cast;
			CastTimer = 0.f;
			SelectedProjectile = nullptr;
		}
		break;
	case FS_Cast:
		if (bIsPress)
		{
			CastTimer += DeltaTime;
			if (CanFireProjectile(HeavyProjectile))
			{
				SelectedProjectile = HeavyProjectile;
			}
			else if (CanFireProjectile(ProjectileTemplate))
			{
				SelectedProjectile = ProjectileTemplate;
				if (HeavyProjectile == nullptr && IsExecutionValid())
				{
					Execute();
				}
			}
		}
		break;
	case FS_Firing:

		TimeToLaunchProjectile += DeltaTime;
		if (TimeToLaunchProjectile >= TimerToLaunchProjectile)
		{
			ShootTimer += DeltaTime;
			if (ShootTimer > SelectedProjectile->TimeByShoot)
			{
				ShootTimer = 0.f;
				LaunchProjectile(SelectedProjectile);
				ShootCount++;
				if (ShootCount >= SelectedProjectile->NbShootByMagazine)
				{
					CurrentState = FS_Recovery;
					CastTimer = 0.f;
					ShootCount = 0;
					if (ManaSystem != nullptr)
						ManaSystem->ConsumeMana(SelectedProjectile->ManaCost);

					bIsExecuting = false;
					RecoveryTimer = SelectedProjectile->RecoveryTime;
					SelectedProjectile = nullptr;
				}
				else
				{
					if (SelectedProjectile->FiringAnim)
					{
						Character->PlayAnimMontage(SelectedProjectile->FiringAnim, SelectedProjectile->AnimRate,
						                           SectionNameRepeat);
					}
				}
			}
		}
		break;
	case FS_Recovery:
		RecoveryTimer -= DeltaTime;
		if (RecoveryTimer <= 0.f)
		{
			CurrentState = FS_None;
		}
		break;
	default: ;
	}


	if (CurrentState != FS_None)
	{
		if (!Character->isAimingMode)
		{
			Interrupt();
		}
	}
}

void UShootAbility::AbilityPressed()
{
	UAbilityBase::AbilityPressed();

	if (CurrentState == FS_None)
	{
		CurrentState = FS_Cast;
		CastTimer = 0.f;
	}
}


void UShootAbility::AbilityRelease()
{
	Super::AbilityRelease();
	if (IsExecutionValid())
	{
		Execute();
	}
}

bool UShootAbility::IsExecutionValid() const
{
	if (!Character->isAimingMode)
	{
		return false;
	}

	if (SelectedProjectile == nullptr)
	{
		return false;
	}

	if (CurrentState != FS_Cast)
	{
		return false;
	}

	return UAbilityBase::IsExecutionValid();
}

void UShootAbility::Execute()
{
	UAbilityBase::Execute();
	if (SelectedProjectile)
	{
		CurrentState = FS_Firing;
		PlayFiringAnim(SelectedProjectile);
		TimeToLaunchProjectile = 0.f;
		ShootTimer = SelectedProjectile->TimeByShoot;
	}
	else
	{
		CurrentState = FS_None;
	}
}

float UShootAbility::GetChargePercent() const
{
	if (HeavyProjectile && HeavyProjectile->CastTime != 0.f)
	{
		return FMath::Clamp(CastTimer / HeavyProjectile->CastTime, 0.f, 1.f);
	}
	return 0.f;
}

void UShootAbility::Interrupt()
{
	if (CurrentState == FS_Firing)
	{
		if (SelectedProjectile && SelectedProjectile->FiringAnim)
		{
			Character->StopAnimMontage(SelectedProjectile->FiringAnim);
		}
		CurrentState = FS_Recovery;
	}
	else
	{
		CurrentState = FS_None;
	}

	if (Character->CharacterState == ECharacterState::STATE_Shoot)
	{
		Character->SetCharacterState(ECharacterState::STATE_None);
	}

	UAbilityBase::Interrupt();
	SelectedProjectile = nullptr;
	CastTimer = 0.f;
	ShootCount = 0;
}

bool UShootAbility::IsHeavyProjectileCasted()
{
	if (HeavyProjectile && SelectedProjectile == HeavyProjectile)
	{
		return true;
	}
	return false;
}

UProjectileTemplate* UShootAbility::GetHeavyProjectileTemplate() const
{
	return HeavyProjectile;
}

void UShootAbility::SetHeavyProjectileTemplate(TSubclassOf<UProjectileTemplate> HeavyProjectileTemplate)
{
	if (HeavyProjectileTemplate)
	{
		if (HeavyProjectile)
		{
			HeavyProjectile->ConditionalBeginDestroy();
			HeavyProjectile = nullptr;
		}
		HeavyProjectile = NewObject<UProjectileTemplate>(Character, HeavyProjectileTemplate);
		if (HeavyProjectile)
			HeavyProjectile->ComputeProperties();
	}
}

float UShootAbility::GetDefaultGravityZ() const
{
	return UPhysicsSettings::Get()->DefaultGravityZ;
}
