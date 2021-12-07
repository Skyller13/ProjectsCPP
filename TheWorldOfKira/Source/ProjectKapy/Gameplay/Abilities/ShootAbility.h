// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbilityActor/Projectile.h"
#include "ProjectKapy/Gameplay/Abilities/AbilityBase.h"
#include "ProjectKapy/Gameplay/Abilities/ShootAbilityBase.h"
#include "ShootAbility.generated.h"

class UProjectileBaseAsset;


/**
 * 
 */
UCLASS()
class PROJECTKAPY_API UShootAbility : public UShootAbilityBase
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UProjectileTemplate> HeavyProjectileSubclass;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	UProjectileTemplate* HeavyProjectile;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	UProjectileTemplate* SelectedProjectile;

public:
	UShootAbility();

public:
	virtual void Init(UAbilityManagerComponent* AbilityMng) override;
	virtual void Tick(float DeltaTime) override;
	virtual void AbilityPressed() override;
	virtual void AbilityRelease() override;
	virtual bool IsExecutionValid() const override;
	virtual void Execute() override;
	virtual float GetChargePercent() const override;
	virtual void Interrupt() override;
	UFUNCTION(BlueprintCallable)
	virtual bool IsHeavyProjectileCasted();

	UFUNCTION(BlueprintCallable)
    UProjectileTemplate* GetHeavyProjectileTemplate() const;

	UFUNCTION(BlueprintCallable)
    void SetHeavyProjectileTemplate(TSubclassOf<UProjectileTemplate> HeavyProjectileTemplate);

	UFUNCTION(BlueprintCallable)
	float GetDefaultGravityZ() const;
};
