// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_FollowPoints.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKAPY_API UBTService_FollowPoints : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	UBTService_FollowPoints(const FObjectInitializer& ObjectInitializer);

	public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
