// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectKapy/Gameplay/Abilities/AbilityBase.h"
#include "DashAbility.generated.h"

class UCatCharacterMovement;
/**
 * 
 */
UCLASS()
class PROJECTKAPY_API UDashAbility : public UAbilityBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cooldown;
	float CooldownTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration;

	UPROPERTY()
	UCatCharacterMovement* MovementComponent;

	UPROPERTY()
	FVector VelocityUntilDash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector DashForward;
	FVector StartDashLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* CurveMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashDistance;

	float LastMovement;

	int32 DashCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NbDash;

	float BreakingFrictionUntilDash;
	float BreakingFrictionDash;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanCauseDamage;

public:
	UDashAbility();
public:

	virtual void Init(UAbilityManagerComponent* AbilityMng) override;
	virtual void Tick(float DeltaTime) override;
	virtual void AbilityPressed() override;
	virtual void Execute() override;
	virtual void EndExecute() override;
	virtual void Interrupt() override;
	virtual bool IsExecutionValid() const override;
	UFUNCTION()
	virtual void OnDashOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                           bool bFromSweep, const FHitResult& SweepResult);
};
