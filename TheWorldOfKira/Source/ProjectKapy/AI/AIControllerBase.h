// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"


#include "AIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKAPY_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()

protected:
	AAIControllerBase();

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	UBlackboardComponent* BlackboardComponent;

	UPROPERTY()
	FTimerHandle TimerHandleLineOfSight;

	UPROPERTY()
	FVector LastPlayerLocation;

	UPROPERTY()
	AActor* LastActor;

	UPROPERTY()
	bool IsNPCLineOfSight = false;

	UFUNCTION()
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void HasStimulus(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void SetLineOfSight(AActor* Actor, bool IsNPC = false);

	UFUNCTION()
	void LastLineOfSight(AActor* Actor);

	UFUNCTION()
	void SetHearing(AActor* Actor);

	UFUNCTION(BlueprintCallable)
    void HasDamageFromPlayer();

	UFUNCTION(BlueprintCallable)
	void UpdatePeripheralVisionAngleDegrees(float Degree);
};
