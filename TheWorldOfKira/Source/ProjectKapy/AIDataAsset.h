// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AIDataAsset.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PROJECTKAPY_API UAIDataAsset : public UDataAsset
{
	GENERATED_BODY()

	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpeedModificator = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LifeModificator = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsHeavy = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsBoss = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsShooting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PercentBuff = 100.0f;
};
