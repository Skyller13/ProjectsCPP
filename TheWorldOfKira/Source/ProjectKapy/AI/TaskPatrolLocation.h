// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "TaskPatrolLocation.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKAPY_API UTaskPatrolLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Setup")
	float PatrolRadius = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Setup")
	FBlackboardKeySelector PatrolLocation;

	UPROPERTY(EditAnywhere, Category = "Setup")
	FBlackboardKeySelector PatrolStartPoint;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UFUNCTION(BlueprintCallable)
	bool PerformPatrolLocation(UBehaviorTreeComponent* OwnerComp);
};
