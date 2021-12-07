// Fill out your copyright notice in the Description page of Project Settings.


#include "KapyCameraModifierFocusTarget.h"

#include "ProjectKapy/KiraCharacter.h"
#include "ProjectKapy/Gameplay/Character/Component/TargetingComponent.h"

bool UKapyCameraModifierFocusTarget::ProcessViewRotation(AActor* ViewTarget, float DeltaTime, FRotator& OutViewRotation,
                                                         FRotator& OutDeltaRot)
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

	AProjectKapyCharacter* currentTarget = KiraCharacter->GetTargetingComponent()->GetTargetedCharacter();

	if (IsValid(currentTarget))
	{
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
			FRotator InterpRot = FMath::RInterpTo(currentRotation, DesiredRotation, DeltaTime,
			                                      SnapSpeed);

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
		return true;
	}
	return false;
}
