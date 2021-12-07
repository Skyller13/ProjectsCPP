// Fill out your copyright notice in the Description page of Project Settings.


#include "DashAbility.h"


#include "Components/CapsuleComponent.h"
#include "ProjectKapy/CatCharacterMovement.h"
#include "ProjectKapy/ProjectKapyCharacter.h"
#include "ProjectKapy/Gameplay/Combat/LifeSystemComponent.h"

UDashAbility::UDashAbility()
{
	ActionName = "Dash";
	DashDistance = 1000.f;
	NbDash = 1;
}

void UDashAbility::Init(UAbilityManagerComponent* AbilityMng)
{
	Super::Init(AbilityMng);
	if (Character)
	{
		MovementComponent = Character->CatCharacterMovement;
	}

	DashCount = NbDash;
}

void UDashAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsExecuting)
	{
		if (ExecutingElapsedTime >= Duration)
		{
			Interrupt();
		}
		else
		{
			if (CurveMovement)
			{
				float Factor = CurveMovement->GetFloatValue(ExecutingElapsedTime / Duration);
				float Movement = Factor * DashDistance;
				float MovementDelta = Movement - LastMovement;
				LastMovement = Movement;
				Character->SetActorLocation(Character->GetActorLocation() + DashForward * MovementDelta, true);
			}
			else
			{
				float Movement = DashDistance * (ExecutingElapsedTime / Duration);
				float MovementDelta = Movement - LastMovement;
				LastMovement = Movement;
				Character->SetActorLocation(Character->GetActorLocation() + DashForward * Movement, true);
			}
		}
	}
	else
	{
		if (DashCount < NbDash)
		{
			CooldownTimer += DeltaTime;
			if (CooldownTimer >= Cooldown)
			{
				DashCount++;
				CooldownTimer = 0.f;
			}
		}
	}
}

void UDashAbility::AbilityPressed()
{
	Super::AbilityPressed();
	if (IsExecutionValid())
	{
		Execute();
	}
}

void UDashAbility::Execute()
{
	if (!bIsExecuting)
	{
		Super::Execute();
		VelocityUntilDash = Character->GetVelocity();
		DashForward = Character->GetActorForwardVector();
		StartDashLocation = Character->GetActorLocation();
		DashCount--;
		BreakingFrictionUntilDash = MovementComponent->BrakingFriction;
		MovementComponent->BrakingFriction = BreakingFrictionDash;
		Character->LifeSystem->CanReceiveDamage = false;
		Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,
		                                                                ECollisionResponse::ECR_Overlap);
		Character->GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &UDashAbility::OnDashOverlap);
		LastMovement = 0.f;
	}
}

void UDashAbility::EndExecute()
{
	Super::EndExecute();
	
}

void UDashAbility::Interrupt()
{
	Super::Interrupt();

	FVector Delta = Character->GetActorLocation() - StartDashLocation;

	if (Character->CharacterState == ECharacterState::STATE_None)
	{
		MovementComponent->Velocity = VelocityUntilDash.Size() * DashForward;
	}
	MovementComponent->BrakingFriction = BreakingFrictionUntilDash;
	Character->LifeSystem->CanReceiveDamage = true;
	Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,
	                                                                ECollisionResponse::ECR_Block);
	Character->GetCapsuleComponent()->OnComponentBeginOverlap.RemoveDynamic(this, &UDashAbility::OnDashOverlap);
	// Character->LaunchCharacter(VelocityUntilDash, true, true);
}

bool UDashAbility::IsExecutionValid() const
{
	if (Character)
	{
		UCatCharacterMovement* Movement = Cast<UCatCharacterMovement>(Character->GetMovementComponent());
		if (Movement &&
			(Movement->CustomMovementMode == EmovementModeCustom::CUSTOM_Climbing
				|| Movement->CustomMovementMode == EmovementModeCustom::CUSTOM_JumpClimbing))
		{
			return false;
		}


		if (Character->CharacterState != ECharacterState::STATE_None)
		{
			return false;
		}
	}

	return Super::IsExecutionValid() && DashCount > 0;
}

void UDashAbility::OnDashOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
	AProjectKapyCharacter* OtherCharacter = Cast<AProjectKapyCharacter>(OtherActor);
	if (OtherCharacter && bCanCauseDamage)
	{
		if (OtherCharacter->GetLifeSystem())
		{
			OtherCharacter->GetLifeSystem()->ApplyDamageAtLocalPos(DashDamage, -DashForward, Character, Character->CharacterType, DT_Melee);
		}
	}
}
