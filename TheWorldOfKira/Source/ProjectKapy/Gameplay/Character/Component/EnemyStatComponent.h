// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyStatComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTKAPY_API UEnemyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEnemyStatComponent();

	//Base Experience depending on level
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* ExperienceCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float experienceReward = 50.f;

	//turn on this one to enable buffed enemies
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsBuff = false;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//Current level 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level = 1;

	//Min level while spawning (player level - this number)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 randomMin = 0;

	//Max level while spawning (player level + this number)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 randomMax = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* AttackCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShootBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* ShootCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HPBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* HealCurve;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void ComputeAttackDamage();
	virtual void ComputeShootDamage();
	virtual void ComputeLife();

	UFUNCTION(BlueprintCallable)
	virtual float GetAttackStats() const;
	UFUNCTION(BlueprintCallable)
	virtual float GetShootStats() const;
	UFUNCTION(BlueprintCallable)
	virtual float GetHPStats() const;
};
