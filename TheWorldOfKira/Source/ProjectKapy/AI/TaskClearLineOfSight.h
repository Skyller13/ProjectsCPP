// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "TaskClearLineOfSight.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKAPY_API UTaskClearLineOfSight : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "BlackboardKey")
	FBlackboardKeySelector LoseLineOfSight;
	
	UPROPERTY(EditDefaultsOnly, Category = "BlackboardKey")
	FBlackboardKeySelector LastPlayerLocation;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UFUNCTION(BlueprintCallable)
    bool PerformClearValues(UBehaviorTreeComponent* OwnerComp);
};
