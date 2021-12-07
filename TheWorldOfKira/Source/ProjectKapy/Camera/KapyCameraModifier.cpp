// Fill out your copyright notice in the Description page of Project Settings.


#include "KapyCameraModifier.h"


#include "KapySpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectKapy/KiraCharacter.h"

bool FCameraInfo::Equals(const FCameraInfo& Other) const
{
	FCameraInfo delta;
	delta.FOV = FOV - Other.FOV;
	delta.SpringArmLength = SpringArmLength - Other.SpringArmLength;
	delta.SocketOffset = SocketOffset - Other.SocketOffset;
	delta.CameraLocation = CameraLocation - Other.CameraLocation;
	return delta.IsZero();
}

bool FCameraInfo::IsZero() const
{
	return FMath::Abs(FOV) < SMALL_NUMBER && FMath::Abs(SpringArmLength) < SMALL_NUMBER
		&& SocketOffset.IsZero() && CameraLocation.IsZero();
}

UKapyCameraModifier::UKapyCameraModifier(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool UKapyCameraModifier::ProcessViewRotation(AActor* ViewTarget, float DeltaTime, FRotator& OutViewRotation,
                                              FRotator& OutDeltaRot)
{
	Super::ProcessViewRotation(ViewTarget, DeltaTime, OutViewRotation, OutDeltaRot);
	return false;
}

bool UKapyCameraModifier::ModifyCamera(float DeltaTime, FMinimalViewInfo& InOutPOV)
{
	Super::ModifyCamera(DeltaTime, InOutPOV);
	return false;
}

void UKapyCameraModifier::ApplyCameraInfo(const FCameraInfo& CameraInfo, const float Factor,
                                          FMinimalViewInfo& InOutPOV)
{
	AActor* viewTarget = GetViewTarget();
	AProjectKapyCharacter* KapyCharacter = Cast<AProjectKapyCharacter>(viewTarget);
	if (!IsValid(KapyCharacter))
	{
		return;
	}

	//Apply Spring Arm Lenght
	UKapySpringArmComponent* SpingArm = KapyCharacter->GetSpringArm();
	if (SpingArm)
	{
		SpingArm->TargetArmLengthModifier += CameraInfo.SpringArmLength * Factor;
		SpingArm->SocketOffsetModifier += CameraInfo.SocketOffset * Factor;
	}

	//Apply FOV
	float deltaFOV = CameraInfo.FOV * Factor;
	InOutPOV.FOV = InOutPOV.FOV + deltaFOV;

	FVector deltaCameraLocation = CameraInfo.CameraLocation * Factor;
	InOutPOV.Location = InOutPOV.Location + deltaCameraLocation;
}

FCameraInfo UKapyCameraModifier::LerpCameraInfo(const FCameraInfo& CameraA, const FCameraInfo& CameraB,
                                                const float Factor) const
{
	if (Factor <= 0.f)
		return CameraA;

	if (Factor >= 1.f)
		return CameraB;

	FCameraInfo LerpCamera;
	LerpCamera.FOV = FMath::Lerp(CameraA.FOV, CameraB.FOV, Factor);
	LerpCamera.SpringArmLength = FMath::Lerp(CameraA.SpringArmLength, CameraB.SpringArmLength, Factor);
	LerpCamera.SocketOffset = FMath::Lerp(CameraA.SocketOffset, CameraB.SocketOffset, Factor);
	
	LerpCamera.CameraLocation = FMath::Lerp(CameraA.CameraLocation, CameraB.CameraLocation, Factor);
	return LerpCamera;
}


void UKapyCameraModifier::ApplyCameraTransition(const FCameraInfo CameraInfo, const float TransitionTime,
                                                FMinimalViewInfo& InOutPOV, const float DeltaTime)
{
	//New Target Modifier
	if (!TargetModifiers.Equals(CameraInfo))
	{
		CurrentModifiers = TargetModifiers;
		TargetModifiers = CameraInfo;

		if (RemainingTransitionTime > 0.0f)
		{
			RemainingTransitionTime = TransitionTime - TransitionTime * (RemainingTransitionTime / TotalTransitionTime);
		}
		else
		{
			RemainingTransitionTime = TransitionTime;
		}
		TotalTransitionTime = TransitionTime;
	}

	//Current Modifiers Already Equals to TargetModifier, No Transition
	if (CurrentModifiers.Equals(TargetModifiers))
	{
		ApplyCameraInfo(CurrentModifiers, 1.0f, InOutPOV);
		return;
	}

	// Tick transition timer.
	RemainingTransitionTime -= DeltaTime;

	if (RemainingTransitionTime <= 0.0f)
	{
		// Transition finished.
		CurrentModifiers = TargetModifiers;
		ApplyCameraInfo(TargetModifiers, 1.f, InOutPOV);
	}
	else
	{
		float Factor = 1 - (RemainingTransitionTime / TotalTransitionTime);
		FCameraInfo ModifiersToApply = LerpCameraInfo(CurrentModifiers, TargetModifiers, Factor);
		ApplyCameraInfo(ModifiersToApply, 1.f, InOutPOV);
	}
}

bool UKapyCameraModifier::IsRotationAccepted(FRotator& Rotator, FRotator& RotAccepted)
{
	return FMath::Abs(Rotator.Pitch) <= RotAccepted.Pitch &&
    FMath::Abs(Rotator.Yaw) <= RotAccepted.Yaw &&
    FMath::Abs(Rotator.Roll) <= RotAccepted.Roll;
}
