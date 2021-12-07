// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityActor/Projectile.h"
#include "ProjectileTemplate.generated.h"


USTRUCT(BlueprintType)
struct FProjectileModifier
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RangeExplosion = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Mana = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Gravity = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Velocity = 1.f;
};

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable,EditInlineNew)
class PROJECTKAPY_API UProjectileTemplate : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AProjectile> ProjectileActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RecoveryTimeBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CastTimeBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ManaCostBase;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	float RecoveryTime;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	float CastTime;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	float ManaCost;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* FiringAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AnimRate = 1.f;

	float AnimNotifTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NbProjectileByShoot = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ConeSizeDegrees;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NbShootByMagazine = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeByShoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FProjectileProperties BaseProperties;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	FProjectileProperties ComputedProperties;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FProjectileModifier Modifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseAimAssist;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector AimOffset;

	UProjectileTemplate();
public:

	UFUNCTION(BlueprintCallable)
	void SetModifier(FProjectileModifier NewModifier);
	UFUNCTION(BlueprintCallable)
	void ComputeProperties();

	bool IsValid() const;
};
