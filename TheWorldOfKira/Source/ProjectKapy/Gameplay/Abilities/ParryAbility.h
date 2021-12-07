// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbilityActor/Projectile.h"
#include "ProjectKapy/Gameplay/Abilities/AbilityBase.h"
#include "ParryAbility.generated.h"
class UAttackAsset;

/**
 * 
 */

USTRUCT(BlueprintType)
struct FParryResult
{
	GENERATED_BODY()
public:
	bool bIsPerfectParry = false;
};

class AStatusEffect;

UCLASS()
class PROJECTKAPY_API UParryAbility : public UAbilityBase
{
	GENERATED_BODY()

protected:
	float ParryElapsedTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ParryPerfectTime;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	int32 ParryCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AnimReact;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	AStatusEffect* BuffInstancied;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanParryPerfectAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanParryPerfectShoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AStatusEffect> EffectBuff;
public:
	UParryAbility();

public:
	virtual void Tick(float DeltaTime) override;
	virtual void AbilityPressed() override;
	virtual void AbilityRelease() override;
	virtual void Interrupt() override;
	virtual void Execute() override;
	virtual bool BlockAttack(FParryResult& ParryResult, UAttackAsset* Attack, UPrimitiveComponent* OverlappedComponent,
	                         FVector InitiatorLocation);
	bool BlockProjectile(FParryResult& ParryResult, AProjectile* Projectile, UPrimitiveComponent* OverlappedComponent);
	void ReflectProjectile(AProjectile* Projectile);
	void AddBuff();
	UFUNCTION()
	void ClearBuff();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnPerfectAttackParry();
};
