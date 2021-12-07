// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateFollowPointsMouseMecha.h"

#include "AIMechanicalMouseController.h"

EBTNodeResult::Type UCreateFollowPointsMouseMecha::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	return  PerformPatrolLocation(&OwnerComp) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}

bool UCreateFollowPointsMouseMecha::PerformPatrolLocation(UBehaviorTreeComponent* OwnerComp)
{
	if(OwnerComp)
		return Cast<AAIMechanicalMouseController>(OwnerComp->GetAIOwner())->CreateFollowPoints();
	return false;
}
