// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraModifier.h"
#include "KapyCameraModifier.generated.h"


USTRUCT(BlueprintType)
struct FCameraInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float FOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float SpringArmLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	FVector SocketOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	FVector CameraLocation;
	
	bool Equals(const FCameraInfo& Other) const;

	bool IsZero() const;
};

/**
 * 
 */
UCLASS()
class PROJECTKAPY_API UKapyCameraModifier : public UCameraModifier
{
	GENERATED_BODY()

private:

	FCameraInfo CurrentModifiers;
	FCameraInfo TargetModifiers;
	float TotalTransitionTime;
	float RemainingTransitionTime;
	
public:
	UKapyCameraModifier(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual bool ProcessViewRotation(class AActor* ViewTarget, float DeltaTime, FRotator& OutViewRotation,
	                                 FRotator& OutDeltaRot) override;
	virtual bool ModifyCamera(float DeltaTime, FMinimalViewInfo& InOutPOV) override;

	void ApplyCameraInfo(const FCameraInfo& CameraInfo, const float Factor, struct FMinimalViewInfo& InOutPOV);

	FCameraInfo LerpCameraInfo(const FCameraInfo& CameraA, const FCameraInfo& CameraB, const float Factor) const;

	void ApplyCameraTransition(const FCameraInfo CameraInfo, const float TransitionTime,
	                           struct FMinimalViewInfo& InOutPOV, const float DeltaTime);

	bool IsRotationAccepted(FRotator& Rotator, FRotator& RotAccepted);
};
