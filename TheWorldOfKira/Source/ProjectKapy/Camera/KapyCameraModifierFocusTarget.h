// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectKapy/Camera/KapyCameraModifier.h"
#include "KapyCameraModifierFocusTarget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKAPY_API UKapyCameraModifierFocusTarget : public UKapyCameraModifier
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SnapSpeed;

	FRotator DesiredRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float HeightLimit = 100.f;

	virtual bool ProcessViewRotation(AActor* ViewTarget, float DeltaTime, FRotator& OutViewRotation,
	                                 FRotator& OutDeltaRot) override;

};
