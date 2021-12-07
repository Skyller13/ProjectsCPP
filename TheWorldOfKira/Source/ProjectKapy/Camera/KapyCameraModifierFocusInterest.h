// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectKapy/Camera/KapyCameraModifier.h"
#include "KapyCameraModifierFocusInterest.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKAPY_API UKapyCameraModifierFocusInterest : public UKapyCameraModifier
{
	GENERATED_BODY()
	public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SnapSpeed;

	FRotator DesiredRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FRotator RotationAccepted;

	float Factor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TimeToRefocus = 2.f;
	float TimeToRefocusRemaining;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float HeightLimit = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float InputCooldown = 1.f;
	float InputCooldownTimer;
	
	bool bTargetIsFocus;

	virtual bool ProcessViewRotation(AActor* ViewTarget, float DeltaTime, FRotator& OutViewRotation,
                                     FRotator& OutDeltaRot) override;
};
