// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_PatrolFinished.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKAPY_API UBTService_PatrolFinished : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	UBTService_PatrolFinished(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(EditAnywhere, Category=Blackboard)
	struct FBlackboardKeySelector BlackboardKeyVector;

	public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
