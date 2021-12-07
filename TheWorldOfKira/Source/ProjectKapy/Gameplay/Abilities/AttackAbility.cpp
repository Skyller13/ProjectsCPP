// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackAbility.h"


#include "ParryAbility.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectKapy/CatCharacterMovement.h"
#include "ProjectKapy/KiraCharacter.h"
#include "ProjectKapy/ProjectKapyCharacter.h"
#include "ProjectKapy/Gameplay/Character/Component/TargetingComponent.h"
#include "ProjectKapy/Gameplay/Combat/EffectReceiverComponent.h"
#include "ProjectKapy/Gameplay/Combat/LifeSystemComponent.h"
#include "ProjectKapy/Gameplay/Combat/ManaSystemComponent.h"
#include "ProjectKapy/Gameplay/DataAsset/AttackAsset.h"

UAttackAbility::UAttackAbility()
{
	AttackCount = 0;
	bCanChainAttack = true;
	bFollowAnimation = false;
	ActionName = "Attack";
	SpeedAttackModifier = 1.f;
}

void UAttackAbility::Init(UAbilityManagerComponent* AbilityMng)
{
	Super::Init(AbilityMng);
}

void UAttackAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Combo > 0)
	{
		ElapsedTimeCombo += DeltaTime;
		if (ElapsedTimeCombo >= TimeComboReset)
		{
			Combo = 0;
		}
	}

	if (bIsExecuting)
	{
		AttackElapsedTime += DeltaTime;
		if (Character)
		{
			UCatCharacterMovement* movement = Cast<UCatCharacterMovement>(Character->GetMovementComponent());
			if ((movement && movement->MovementMode != MOVE_Walking) || Character->isAimingMode || Character->
				CharacterState != STATE_Melee)
			{
				Interrupt();
				return;
			}
		}

		if (bIsBlocked && ParryResult.bIsPerfectParry)
		{
			Interrupt();
			Character->GetEffectReceiver()->AddEffectByClass(StunEffect);
			return;
		}

		if (bFollowAnimation)
		{
			if (CurveMovement && HitboxActiveTimer != 0.f)
			{
				// float Velocity = CurveMovement->GetFloatValue(AttackElapsedTime / HitboxActiveTimer) * DeltaTime;
				float Movement = CurveMovement->GetFloatValue(AttackElapsedTime / HitboxActiveTimer);
				float DeltaMov = Movement - LastMovement;
				LastMovement = Movement;
				if (Character)
				{
					Character->SetActorLocation(
						Character->GetActorLocation() + AttackFoward * DeltaMov, true);
				}
			}

			if (ResetTimer <= AttackElapsedTime)
			{
				ResetAttack();
			}

			if (HitboxActor == nullptr && AttackElapsedTime >= HitboxActiveTimer && AttackElapsedTime <
				HitboxDisableTimer)
			{
				SpawnHitbox(CurrentAttack);
			}
			else if (HitboxActor && AttackElapsedTime >= HitboxDisableTimer)
			{
				DestroyHitbox();
				if (!bAttackHit)
				{
					Combo = 0;
				}
			}
		}
	}
	else if (bIsRecovering)
	{
		TimeRecoveryRemaining -= DeltaTime;
		if (TimeRecoveryRemaining <= 0.f)
		{
			bIsRecovering = false;
		}
	}
}

void UAttackAbility::AbilityPressed()
{
	Super::AbilityPressed();
	if (IsExecutionValid())
	{
		Execute();
	}
}

void UAttackAbility::Execute()
{
	Super::Execute();

	AKiraCharacter* KiraCharacter = Cast<AKiraCharacter>(Character);

	if (KiraCharacter)
	{
		if (KiraCharacter->GetTargetingComponent()->GetTargetedCharacter() != nullptr)
		{
			Target = KiraCharacter->GetTargetingComponent()->GetTargetedCharacter();
			FVector ToTarget = Target->GetActorLocation() - Character->GetActorLocation();
			ToTarget = ToTarget.VectorPlaneProject(ToTarget, FVector::UpVector);
			AttackFoward = ToTarget.GetSafeNormal();
		}
		else
		{
			AttackFoward = Character->GetCameraComponent()->GetForwardVector();
			AttackFoward = AttackFoward.VectorPlaneProject(AttackFoward, FVector::UpVector);
			AttackFoward.Normalize();
		}

		Character->SetActorRotation(AttackFoward.Rotation());
	}
	else
	{
		AttackFoward = Character->GetActorForwardVector();
	}

	CharacterLocation = Character->GetActorLocation();

	ProcessAttack();
	// }
}

void UAttackAbility::Interrupt()
{
	Super::Interrupt();

	// if (!bIsExecuting)
	// 	return;

	if (Character)
	{
		if (Character->CharacterState == STATE_Melee)
		{
			Character->SetCharacterState(ECharacterState::STATE_None);
		}
		if (CurrentAttack)
			Character->StopAnimMontage(CurrentAttack->AnimMontage);
		bFollowAnimation = false;
	}

	if (IsValid(HitboxActor))
	{
		HitboxActor->OnAttackInterrupted();
	}

	DestroyHitbox();
	CurrentAttack = nullptr;
	AttackCount = 0;
	AttackHitCount = 0;
	bIsExecuting = false;
	bCanChainAttack = true;
	AttackElapsedTime = 0.f;
	bIsBlocked = false;
	bIsRecovering = true;
	TimeRecoveryRemaining = TimeRecovery;
}

bool UAttackAbility::IsExecutionValid() const
{
	if (AbilityManager)
	{
		UAbilityBase* CurrentAbility = AbilityManager->GetAbilityExecuting();
		if (CurrentAbility != nullptr && CurrentAbility != this)
		{
			return false;
		}
	}

	if (Character)
	{
		UCatCharacterMovement* movement = Cast<UCatCharacterMovement>(Character->GetMovementComponent());
		if (movement && movement->MovementMode != MOVE_Walking)
		{
			return false;
		}
	}

	if (AttackCount >= AttacksArray.Num() || !bCanChainAttack)
	{
		return false;
	}

	if (Character->isAimingMode)
	{
		return false;
	}

	if (bIsRecovering)
	{
		return false;
	}

	if (Character->CharacterState != ECharacterState::STATE_Melee &&
		Character->CharacterState != ECharacterState::STATE_None)
	{
		return false;
	}


	if (AttacksArray[AttackCount] != nullptr && !Character->ManaSystem->HaveEnoughtMana(
		AttacksArray[AttackCount]->ManaCost))
	{
		return false;
	}

	return true;
}

void UAttackAbility::OnAbilityBeginOverlap(AAbilityActorBase* AbilityActor, UPrimitiveComponent* OverlappedComponent,
                                           AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                           bool bFromSweep,
                                           const FHitResult& SweepResult)
{
	if (OtherActor == Character || CurrentAttack == nullptr)
		return;


	if (!IsValid(OtherActor))
	{
		return;
	}

	ULifeSystemComponent* LifeSystemComponent = OtherActor->FindComponentByClass<ULifeSystemComponent>();
	if (LifeSystemComponent == nullptr)
	{
		return;
	}

	if (!LifeSystemComponent->IsDamageSensible(Character->CharacterType))
	{
		return;
	}

	Super::OnAbilityBeginOverlap(AbilityActor, OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep,
	                             SweepResult);

	AProjectKapyCharacter* KapyCharacter = Cast<AProjectKapyCharacter>(OtherActor);

	if (KapyCharacter)
	{
		UParryAbility* OtherParryAbility = Cast<UParryAbility>(KapyCharacter->AbilityManager->GetAbilityExecuting());
		if (OtherParryAbility && OtherParryAbility->BlockAttack(ParryResult, CurrentAttack, OverlappedComponent,
		                                                        Character->GetActorLocation()))
		{
			bIsBlocked = true;
		}
	}

	if (!bIsBlocked)
	{
		ApplyDamageToActor(AbilityActor, OverlappedComponent, OtherActor, OtherComp);
	}
}

void UAttackAbility::ProcessAttack()
{
	CurrentAttack = AttacksArray[AttackCount];
	if (Character && CurrentAttack)
	{
		Character->SetCharacterState(ECharacterState::STATE_Melee);

		Character->PlayAnimMontage(CurrentAttack->AnimMontage, CurrentAttack->AnimSpeed * SpeedAttackModifier);

		for (FAnimNotifyEvent Notify : CurrentAttack->AnimMontage->Notifies)
		{
			if (Notify.NotifyName == "ResetAttack")
			{
				ResetTimer = Notify.GetTime() / (CurrentAttack->AnimSpeed * SpeedAttackModifier);
			}
			else if (Notify.NotifyName == "HitboxAttackEnable_ANot_C")
			{
				HitboxActiveTimer = Notify.GetTime() / (CurrentAttack->AnimSpeed * SpeedAttackModifier);
				HitboxDisableTimer = HitboxActiveTimer + (Notify.GetDuration() / (CurrentAttack->AnimSpeed *
					SpeedAttackModifier));
			}
		}
		bFollowAnimation = true;

		Character->ManaSystem->ConsumeMana(CurrentAttack->ManaCost);
		
	}

	AttackCount++;
	if (bAttackLoop && AttackCount == AttacksArray.Num())
	{
		AttackCount = 0;
	}
	bCanChainAttack = false;
	bIsExecuting = true;
	AttackElapsedTime = 0.f;
	bAttackHit = false;
	LastMovement = 0.f;
}

bool UAttackAbility::AttackIsValid()
{
	if (Character)
	{
		UCatCharacterMovement* movement = Cast<UCatCharacterMovement>(Character->GetMovementComponent());
		if (movement && movement->MovementMode != MOVE_Walking)
		{
			return false;
		}
	}

	if (AttackCount >= AttacksArray.Num() || !bCanChainAttack)
	{
		return false;
	}

	if (Character->isAimingMode)
	{
		return false;
	}

	if (Character->CharacterState != ECharacterState::STATE_Melee && Character->CharacterState !=
		ECharacterState::STATE_None)
	{
		return false;
	}

	return true;
}

void UAttackAbility::SetCanChainAttack(bool CanChain)
{
	bCanChainAttack = CanChain;
}

void UAttackAbility::ResetAttack()
{
	Interrupt();
	Target = nullptr;
}

void UAttackAbility::ApplyDamageToActor(AAbilityActorBase* AbilityActor, UPrimitiveComponent* OverlappedComponent,
                                        AActor* OtherActor, UPrimitiveComponent* OtherComp)
{
	ULifeSystemComponent* OtherLifeSystem = OtherActor->FindComponentByClass<ULifeSystemComponent>();
	if (OtherLifeSystem == nullptr)
		return;


	FHitResult OutHit;
	FCollisionQueryParams CollisionParameters = FCollisionQueryParams::DefaultQueryParam;
	CollisionParameters.AddIgnoredActor(Character);
	CollisionParameters.AddIgnoredActor(AbilityActor);
	FVector LocStart = Character->GetMesh()->GetSocketLocation(CurrentAttack->HitboxSocket);


	float Damage = CurrentAttack->DamageBase * DamageModifier * OtherLifeSystem->MeleeDamageModifier;
	float ComboFactor = ((float)FMath::Clamp(Combo, 0, NbComboMaxDamage) / (float)NbComboMaxDamage);
	Damage += Damage * (ComboFactor * ComboDamageModifier);

	if (GetWorld()->LineTraceSingleByChannel(OutHit, LocStart,
	                                         OtherComp->GetComponentLocation(),
	                                         ECollisionChannel::ECC_GameTraceChannel1, CollisionParameters))
	{
		BP_OnAttackHit(OutHit, AbilityActor, OverlappedComponent, OtherActor, OtherComp);
		OtherLifeSystem->ApplyDamageAtPos(Damage, OutHit.ImpactPoint, AbilityActor, Character->CharacterType, DT_Melee);
	}
	else
	{
		OtherLifeSystem->ApplyDamageAtPos(Damage, AbilityActor->GetActorLocation(), AbilityActor,
		                                  Character->CharacterType, DT_Melee);
	}

	ApplyEffectToActor(OtherActor, CurrentAttack->Effect);

	if (!bAttackHit)
	{
		bAttackHit = true;
		AttackHitCount += 1;
		Combo += 1;
		ElapsedTimeCombo = 0.f;
	}

	if (AttackHitCount == AttacksArray.Num() && ComboEffect)
	{
		ApplyEffectToActor(OtherActor, ComboEffect);
	}
}

void UAttackAbility::ApplyEffectToActor(AActor* Actor, TSubclassOf<AStatusEffect> Effect)
{
	UEffectReceiverComponent* OtherEffectReceiver = Actor->FindComponentByClass<UEffectReceiverComponent>();
	if (Effect && OtherEffectReceiver)
	{
		OtherEffectReceiver->AddEffectByClass(Effect);
	}
}

void UAttackAbility::OnCharacterReceiveDamage(AActor* DamagedActor, float Value, AActor* DamageCauser)
{
	Combo = 0;
	if (bSensitiveDamage)
	{
		Interrupt();
	}
}

void UAttackAbility::SpawnHitbox(UAttackAsset* Attack)
{
	DestroyHitbox();

	if (Attack && Attack->Hitbox)
	{
		FTransform Transform = Character->GetActorTransform();

		if (!Attack->HitboxSocket.IsNone())
		{
			Transform = Character->GetMesh()->GetSocketTransform(Attack->HitboxSocket);
		}
		HitboxActor = AbilityManager->GetWorld()->SpawnActorDeferred<AHitboxAbilityActor>(
			Attack->Hitbox, Transform, Character);
		HitboxActor->Init(this);
		HitboxActor->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::KeepWorldTransform,
		                               Attack->HitboxSocket);
		HitboxActor->FinishSpawning(Transform);
	}
}

void UAttackAbility::DestroyHitbox()
{
	if (HitboxActor != nullptr)
	{
		HitboxActor->Destroy();
		HitboxActor = nullptr;
	}
}
