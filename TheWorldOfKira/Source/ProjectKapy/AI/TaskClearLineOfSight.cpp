// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskClearLineOfSight.h"

#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UTaskClearLineOfSight::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{    
	Super::ExecuteTask(OwnerComp, NodeMemory);
    return  PerformClearValues(&OwnerComp) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}

bool UTaskClearLineOfSight::PerformClearValues(UBehaviorTreeComponent* OwnerComp)
{
	if(OwnerComp)
	{
		UBlackboardComponent* BlackboardComponent = OwnerComp->GetBlackboardComponent();
		LoseLineOfSight.SelectedKeyName = "LoseLineOfSight";
		LastPlayerLocation.SelectedKeyName = "LastPlayerLocation";
		if(BlackboardComponent)
		{
			BlackboardComponent->ClearValue(LoseLineOfSight.SelectedKeyName);
			BlackboardComponent->ClearValue(LastPlayerLocation.SelectedKeyName);
			return true;
		}
	}
	return false;
}

