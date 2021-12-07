// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectKapy/Camera/KapyCameraModifier.h"
#include "KapyCameraModifierRun.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKAPY_API UKapyCameraModifierRun : public UKapyCameraModifier
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCameraInfo Modifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TransitionTime = 0.5f;

public:
	virtual bool ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV) override;
};
