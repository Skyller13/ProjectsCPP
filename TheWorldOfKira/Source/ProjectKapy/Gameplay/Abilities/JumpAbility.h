// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectKapy/Gameplay/Abilities/AbilityBase.h"
#include "JumpAbility.generated.h"

class UCatCharacterMovement;
/**
 * 
 */
UCLASS()
class PROJECTKAPY_API UJumpAbility : public UAbilityBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 JumpCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NbJumbMax = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpHeight = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FirstJumpHeight = 600.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DelayFirstJump = 0.3f;
	FTimerHandle FirstJumpHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AnimJumpImpulse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AnimJumpImpulseRate = 1.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AnimDoubleJump;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AnimDoubleJumpRate = 1.5f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bPrepareJump;

	UPROPERTY
	(VisibleInstanceOnly, BlueprintReadWrite)
	float HeightMaxReached;
	UPROPERTY(BlueprintReadWrite)
	float HeightFallen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bJumpResetHeightFallen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bFallDamageEnable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeightMinNoDamage = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FallDamageBase = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamagePerMeter = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeFallRecovery = 0.5f;

	float FallRecoveryRemaining;

	UPROPERTY(BlueprintReadWrite)
	float AirControl = 0.2f;
	
	UPROPERTY()
	UCatCharacterMovement* CatMovement;

public:
	UJumpAbility();

	virtual void Init(UAbilityManagerComponent* AbilityMng) override;
	virtual void Tick(float DeltaTime) override;

	virtual void AbilityPressed() override;
	virtual void Execute() override;
	virtual bool IsExecutionValid() const override;

	UFUNCTION()
	void ExecuteFirstJump();

	UFUNCTION(BlueprintCallable)
	virtual bool IsPreparingJump() const;

	virtual void Landed(const FHitResult& _hit);
	UFUNCTION(BlueprintImplementableEvent)
	void BP_Landed(const FHitResult& _hit);
};
