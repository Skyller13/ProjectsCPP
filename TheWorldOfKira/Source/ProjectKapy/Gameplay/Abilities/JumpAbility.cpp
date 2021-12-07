// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpAbility.h"


#include "ProjectKapy/CatCharacterMovement.h"
#include "ProjectKapy/ProjectKapyCharacter.h"
#include "ProjectKapy/Gameplay/Combat/LifeSystemComponent.h"

UJumpAbility::UJumpAbility()
{
	ActionName = "Jump";
}

void UJumpAbility::Init(UAbilityManagerComponent* AbilityMng)
{
	Super::Init(AbilityMng);
	if (Character && Character->GetMovementComponent())
	{
		CatMovement = Cast<UCatCharacterMovement>(Character->GetMovementComponent());
	}
}

void UJumpAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CatMovement == nullptr)
		return;

	if (CatMovement->IsFalling())
	{
		float CurrentHeight = Character->GetActorLocation().Z;
		if (CurrentHeight > HeightMaxReached)
		{
			HeightMaxReached = CurrentHeight;
		}
	}
	else
	{
		HeightMaxReached = Character->GetActorLocation().Z;
	}

	HeightFallen = HeightMaxReached - Character->GetActorLocation().Z;


	if (Character->CharacterState == STATE_FallDamage)
	{
		FallRecoveryRemaining -= DeltaTime;
		if (FallRecoveryRemaining <= 0.f)
		{
			Character->SetCharacterState(STATE_None);
		}
	}
}

void UJumpAbility::AbilityPressed()
{
	Super::AbilityPressed();
	if (IsExecutionValid())
	{
		Execute();
	}
}

void UJumpAbility::Execute()
{
	Super::Execute();

	if (CatMovement->IsFalling() && JumpCount == 0)
	{
		JumpCount = 1;
	}

	if (JumpCount < NbJumbMax)
	{
		if (JumpCount == 0 && !GetWorld()->GetTimerManager().IsTimerActive(FirstJumpHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(FirstJumpHandle, this, &UJumpAbility::ExecuteFirstJump,
			                                       DelayFirstJump, false);
			if (Character && AnimJumpImpulse)
			{
				Character->PlayAnimMontage(AnimJumpImpulse, AnimJumpImpulseRate, "Jump");
			}
		}
		else
		{
			if (JumpCount != 0)
			{
				if (Character && AnimDoubleJump)
				{
					Character->PlayAnimMontage(AnimDoubleJump, AnimDoubleJumpRate, "Jump");
				}
			
				Character->LaunchCharacter(FVector(0, 0, JumpHeight), false, true);
				JumpCount++;
			}
			
		}

		CatMovement->AirControl = AirControl;
	}

	if (bJumpResetHeightFallen)
	{
		HeightMaxReached = Character->GetActorLocation().Z;
	}

	EndExecute();
}

bool UJumpAbility::IsExecutionValid() const
{
	if (CatMovement == nullptr)
	{
		return false;
	}

	if (CatMovement->bIsPushingBlock || CatMovement->CustomMovementMode != EmovementModeCustom::CUSTOM_None)
	{
		return false;
	}

	if (JumpCount >= NbJumbMax)
	{
		return false;
	}

	return Super::IsExecutionValid();
}

void UJumpAbility::ExecuteFirstJump()
{
	if (IsExecutionValid())
	{
		Character->LaunchCharacter(FVector(0, 0, FirstJumpHeight), false, true);
		GetWorld()->GetTimerManager().ClearTimer(FirstJumpHandle);
		bPrepareJump = false;
		JumpCount++;
	}
}

bool UJumpAbility::IsPreparingJump() const
{
	return bPrepareJump;
}

void UJumpAbility::Landed(const FHitResult& _hit)
{
	JumpCount = 0.f;
	HeightFallen = HeightMaxReached - Character->GetActorLocation().Z;

	if (bFallDamageEnable)
	{
		if (HeightFallen > HeightMinNoDamage)
		{
			float Damage = FallDamageBase;
			float Meter = HeightFallen - HeightMinNoDamage;
			Meter = FMath::FloorToFloat(Meter / 100.f);
			Damage += Meter * DamagePerMeter;
			Character->SetCharacterState(ECharacterState::STATE_FallDamage);
			Character->LifeSystem->ApplyDamageAtPos(Damage, _hit.Location, nullptr, ECharacterType::CT_None);
			FallRecoveryRemaining = TimeFallRecovery;
		}
	}
	BP_Landed(_hit);
}
