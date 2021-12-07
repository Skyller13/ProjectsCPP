// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectKapy/Camera/KapyCameraModifier.h"
#include "KapyCameraModifierWalkDirection.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKAPY_API UKapyCameraModifierWalkDirection : public UKapyCameraModifier
{
	GENERATED_BODY()

protected:
	float PreviousYawDeltaSign;

	float DirectionChangeCooldownRemaining;

	float NoViewInputRemainingTime;
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SnapSpeed = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TransitionTime = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DirectionChangeCooldown = 0.5f;
	


public:
	virtual bool ProcessViewRotation(AActor* ViewTarget, float DeltaTime, FRotator& OutViewRotation,
	                                 FRotator& OutDeltaRot) override;
};
