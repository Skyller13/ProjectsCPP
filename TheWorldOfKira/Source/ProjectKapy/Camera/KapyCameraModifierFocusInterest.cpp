// Fill out your copyright notice in the Description page of Project Settings.


#include "KapyCameraModifierFocusInterest.h"

#include "ProjectKapy/KiraCharacter.h"

bool UKapyCameraModifierFocusInterest::ProcessViewRotation(AActor* ViewTarget, float DeltaTime,
                                                           FRotator& OutViewRotation, FRotator& OutDeltaRot)
{
	Super::ProcessViewRotation(ViewTarget, DeltaTime, OutViewRotation, OutDeltaRot);
	AKiraCharacter* KiraCharacter = Cast<AKiraCharacter>(ViewTarget);


	if (!IsValid(KiraCharacter))
	{
		return false;
	}

	if (KiraCharacter->isAimingMode)
	{
		return false;
	}


	if (TimeToRefocusRemaining > 0.f && InputCooldownTimer >= InputCooldown)
	{
		TimeToRefocusRemaining -= DeltaTime;
		if (TimeToRefocusRemaining < 0.f)
		{
			TimeToRefocusRemaining = 0.f;
		}
	}

	APlayerController* PlayerController = Cast<APlayerController>(KiraCharacter->GetController());
	AActor* currentTarget = KiraCharacter->Target;

	if (IsValid(currentTarget))
	{
		if (bTargetIsFocus)
		{
			InputCooldownTimer += DeltaTime;

			if (!PlayerController->RotationInput.IsNearlyZero())
			{
				TimeToRefocusRemaining = TimeToRefocus;
				InputCooldownTimer = 0.f;
			}
			if (InputCooldownTimer >= InputCooldown)
			{
				InputCooldownTimer = InputCooldown;
				Factor = 1.f;
			}
			else
			{
				Factor = 0.f;
			}
		}
		else
		{
			Factor = 1.f;
		}

		FVector cameraLocation = CameraOwner->GetCameraLocation();
		FVector targetLocation = currentTarget->GetActorLocation();
		FVector direction = targetLocation - cameraLocation;
		direction.Z = FMath::Clamp(direction.Z, -HeightLimit, HeightLimit);
		DesiredRotation = FRotationMatrix::MakeFromX(direction).Rotator();
		FRotator currentRotation = OutViewRotation;

		if (SnapSpeed <= 0)
		{
			OutViewRotation = DesiredRotation;
		}
		else
		{
			if (Factor > 0.f)
			{
				FRotator InterpRot = FMath::RInterpTo(currentRotation, DesiredRotation, DeltaTime,
				                                      SnapSpeed * Factor);

				if (!bTargetIsFocus)
				{
					FRotator deltaToDesiredRot = DesiredRotation - currentRotation;
					deltaToDesiredRot.Normalize();
					if (IsRotationAccepted(deltaToDesiredRot, RotationAccepted))
					{
						bTargetIsFocus = true;
					}
				}

				FRotator deltaRot = InterpRot - currentRotation;
				deltaRot.Normalize();

				if (deltaRot.IsNearlyZero())
				{
					OutViewRotation = DesiredRotation;
				}
				else
				{
					OutDeltaRot += deltaRot;
				}
			}
		}
		return true;
	}
	else
	{
		bTargetIsFocus = false;
		TimeToRefocusRemaining = TimeToRefocus;
		InputCooldownTimer = InputCooldown;
	}

	return false;
}
