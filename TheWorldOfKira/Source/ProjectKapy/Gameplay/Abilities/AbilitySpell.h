// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectKapy/Gameplay/Abilities/AbilityBase.h"
#include "AbilitySpell.generated.h"

class USpellAsset;
class AStatusEffect;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSpellAbilityDelegateSignature);

UCLASS()
class PROJECTKAPY_API UAbilitySpell : public UAbilityBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AStatusEffect> StatusEffect;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	AStatusEffect* EffectInstancied;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cooldown;
	float CooldownTimer;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	bool bInCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CastTime;
	float CastTimer;

	float EffectTime;
	float EffectTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DurationModifier = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PowerModifier = 1.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bEffectInProgress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bForceEffectDestroy;

public:

	UPROPERTY(BlueprintAssignable)
	FSpellAbilityDelegateSignature OnSpellStartCasting;
	UPROPERTY(BlueprintAssignable)
	FSpellAbilityDelegateSignature OnSpellEndCasting;
	UPROPERTY(BlueprintAssignable)
	FSpellAbilityDelegateSignature OnSpellStartCooldown;
	UPROPERTY(BlueprintAssignable)
	FSpellAbilityDelegateSignature OnSpellEndCooldown;

public:
	UAbilitySpell();
public:
	virtual void Tick(float DeltaTime) override;
	virtual void AbilityPressed() override;
	virtual void AbilityRelease() override;
	virtual void Interrupt() override;
	virtual void Execute() override;
	virtual void ApplyEffect();
	virtual void StartCooldown();
	virtual bool IsExecutionValid() const override;

	UFUNCTION(BlueprintCallable)
	virtual float GetCastChargePercent() const;

	UFUNCTION(BlueprintCallable)
	virtual float GetCooldownPercent() const;

	UFUNCTION(BlueprintCallable)
	virtual void SetStatusEffect(TSubclassOf<AStatusEffect> NewEffect);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsInCooldown() const { return bInCooldown; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCastTime() const { return CastTime; }

	virtual  void OnCharacterReceiveDamage(AActor* DamagedActor, float Value, AActor* DamageCauser) override;
	
};
