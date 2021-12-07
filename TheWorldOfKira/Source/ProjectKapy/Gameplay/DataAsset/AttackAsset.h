// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "AttackAsset.generated.h"

class AHitboxAbilityActor;
class AStatusEffect;

UENUM(BlueprintType)
enum AttackCategory
{
	e_Light = 0 UMETA(DisplayName = "Light"),
	e_Heavy = 1 UMETA(DisplayName = "Heavy"),
	MAX = 2 UMETA(DisplayName = "MAX")
};

/**
 * 
 */

UCLASS()
class PROJECTKAPY_API UAttackAsset : public UDataAsset
{
	GENERATED_BODY()
public :

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AnimSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<AttackCategory> Category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ManaCost;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName HitboxSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AHitboxAbilityActor> Hitbox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AStatusEffect> Effect;
};
