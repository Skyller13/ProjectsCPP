// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackAsset.h"
#include "HeavyAttackAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKAPY_API UHeavyAttackAsset : public UAttackAsset
{
	GENERATED_BODY()
	public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* animCharge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float chargeTime;
};
