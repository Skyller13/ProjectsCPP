// Fill out your copyright notice in the Description page of Project Settings.


#include "KapyCameraModifierWalkDirection.h"


#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectKapy/KiraCharacter.h"

bool UKapyCameraModifierWalkDirection::ProcessViewRotation(class AActor* ViewTarget, float DeltaTime,
                                                           FRotator& OutViewRotation, FRotator& OutDeltaRot)
{
	Super::ProcessViewRotation(ViewTarget, DeltaTime, OutViewRotation, OutDeltaRot);


	if (DirectionChangeCooldownRemaining > 0)
	{
		DirectionChangeCooldownRemaining -= DeltaTime;
	}

	if (NoViewInputRemainingTime > 0)
	{
		NoViewInputRemainingTime -= DeltaTime;
	}


	AKiraCharacter* KiraCharacter = Cast<AKiraCharacter>(ViewTarget);
	if (!IsValid(KiraCharacter))
	{
		return false;
	}

	if (KiraCharacter->isAimingMode)
	{
		return false;
	}

	APlayerController* PlayerController = Cast<APlayerController>(KiraCharacter->GetController());


	UE_LOG(LogTemp, Warning, TEXT("%s"), *KiraCharacter->GetMovementComponent()->GetLastInputVector().ToString());

	if (PlayerController && !PlayerController->RotationInput.IsNearlyZero())
	{
		NoViewInputRemainingTime = TransitionTime;
	}

	if (NoViewInputRemainingTime > 0.f)
	{
		return false;
	}
	else
	{
		const float actorYaw = ViewTarget->GetActorRotation().Yaw;
		const float viewYaw = OutViewRotation.Yaw;

		FRotator RotatorDelta;
		RotatorDelta.Yaw = actorYaw - viewYaw;
		RotatorDelta = RotatorDelta.GetNormalized();


		// float yawDelta = actorYaw - viewYaw;


		// while (yawDelta < -180.0f)
		// {
		// 	yawDelta += 360.0f;
		// }
		//
		// while (yawDelta > 180.0f)
		// {
		// 	yawDelta -= 360.0f;
		// }

		float yawDeltaSign = FMath::Sign(RotatorDelta.Yaw);

		if (PreviousYawDeltaSign != yawDeltaSign)
		{
			if (DirectionChangeCooldownRemaining > 0)
			{
				return false;
			}
			else
			{
				PreviousYawDeltaSign = yawDeltaSign;
				DirectionChangeCooldownRemaining = DirectionChangeCooldown;
			}
		}

		if (FMath::Abs(RotatorDelta.Yaw) <= 90.f)
		{
			float appliedYawDelta = yawDeltaSign * SnapSpeed * DeltaTime;
			if (FMath::Abs(RotatorDelta.Yaw) < FMath::Abs(appliedYawDelta))
			{
				PreviousYawDeltaSign = 0.0f;
				DirectionChangeCooldownRemaining = DirectionChangeCooldown;
				appliedYawDelta = RotatorDelta.Yaw;
			}
			OutDeltaRot.Yaw += appliedYawDelta;
		}
	}


	return false;
}
