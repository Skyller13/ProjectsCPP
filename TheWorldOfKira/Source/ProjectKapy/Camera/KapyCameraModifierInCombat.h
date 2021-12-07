// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectKapy/Camera/KapyCameraModifier.h"
#include "KapyCameraModifierInCombat.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKAPY_API UKapyCameraModifierInCombat : public UKapyCameraModifier
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FCameraInfo Modifiers;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float CombatTransitionTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TimeToReturnNormal;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float ReturnNormalTransitionTime;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	float RemainingTimeNoCombat;
	
public:

	virtual bool ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV) override;
};
