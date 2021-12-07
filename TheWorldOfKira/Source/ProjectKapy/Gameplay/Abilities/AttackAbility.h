// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "ParryAbility.h"
#include "AbilityActor/HitboxAbilityActor.h"
#include "ProjectKapy/Gameplay/Abilities/AbilityBase.h"
#include "AttackAbility.generated.h"

class UAttackAsset;
class AHitboxAbilityActor;
class AStatusEffect;
/**
 * 
 */
UCLASS()
class PROJECTKAPY_API UAttackAbility : public UAbilityBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 AttackCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Combo)
	int32 AttackHitCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Combo)
	int32 Combo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combo)
	int32 NbComboMaxDamage = 10;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Combo)
	float ElapsedTimeCombo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combo)
	float TimeComboReset;

	bool bAttackHit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UAttackAsset*> AttacksArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAttackAsset* CurrentAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bCanChainAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAttackLoop = true;

	bool bFollowAnimation;
	float AttackElapsedTime;
	float ResetTimer;

	float HitboxActiveTimer;
	float HitboxDisableTimer;

	bool bIsBlocked = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AStatusEffect> StunEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AStatusEffect> ComboEffect;

	UPROPERTY()
	AHitboxAbilityActor* HitboxActor;

	UPROPERTY(EditAnywhere, Category = Curve)
	UCurveFloat* CurveMovement;

	float LastMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = Modifier)
	float SpeedAttackModifier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = Modifier)
	float DamageModifier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = Modifier)
	float ComboDamageModifier = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeRecovery;
	float TimeRecoveryRemaining;

	FParryResult ParryResult;

	bool bIsRecovering;

	FVector AttackFoward;
	FVector CharacterLocation;

	const TArray<TEnumAsByte<EObjectTypeQuery>> TargetObjectTypes;

	UPROPERTY()
	AProjectKapyCharacter* Target;

public:
	UAttackAbility();

protected:

	virtual void ProcessAttack();
	virtual bool AttackIsValid();

public:
	virtual void Init(UAbilityManagerComponent* AbilityMng) override;
	virtual void Tick(float DeltaTime) override;
	virtual void AbilityPressed() override;
	virtual void Execute() override;
	virtual void Interrupt() override;
	virtual bool IsExecutionValid() const override;

	virtual void OnAbilityBeginOverlap(AAbilityActorBase* AbilityActor, UPrimitiveComponent* OverlappedComponent,
	                                   AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                                   bool bFromSweep,
	                                   const FHitResult& SweepResult) override;

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnAttackHit(FHitResult OutHit, AAbilityActorBase* AbilityActor, UPrimitiveComponent* OverlappedComponent,
	                    AActor* OtherActor,
	                    UPrimitiveComponent* OtherComp);

	UFUNCTION(BlueprintCallable)
	virtual void SetCanChainAttack(bool CanChain);
	UFUNCTION(BlueprintCallable)
	virtual void ResetAttack();
	void ApplyDamageToActor(AAbilityActorBase* AbilityActor, UPrimitiveComponent* OverlappedComponent,
	                        AActor* OtherActor, UPrimitiveComponent* OtherComp);

	void ApplyEffectToActor(AActor* Actor, TSubclassOf<AStatusEffect> Effect);

	virtual void OnCharacterReceiveDamage(AActor* DamagedActor, float Value, AActor* DamageCauser) override;

	virtual void SpawnHitbox(UAttackAsset* Attack);
	virtual void DestroyHitbox();

	FORCEINLINE void SetAttackModifier(float _atkModifier) { DamageModifier = _atkModifier; }
};
