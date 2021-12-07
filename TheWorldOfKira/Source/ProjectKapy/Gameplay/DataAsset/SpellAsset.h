// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SpellAsset.generated.h"
class AStatusEffect;
/**
 * 
 */
UCLASS()
class PROJECTKAPY_API USpellAsset : public UDataAsset
{
	GENERATED_BODY()

public :

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AnimSpeed = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AnimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CastTime = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AStatusEffect> Effect;
};
