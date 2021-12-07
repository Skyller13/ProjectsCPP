// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskPatrolLocation.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UTaskPatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{    
    Super::ExecuteTask(OwnerComp, NodeMemory);
    return  PerformPatrolLocation(&OwnerComp) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}

bool UTaskPatrolLocation::PerformPatrolLocation(UBehaviorTreeComponent* OwnerComp)
{
	if(OwnerComp)
	{
		FNavLocation Location;
		UBlackboardComponent* BlackboardComponent = OwnerComp->GetBlackboardComponent();
		const FVector StartPoint = BlackboardComponent->GetValueAsVector(PatrolStartPoint.SelectedKeyName);
    
		ANavigationData* NavData = Cast<ANavigationData>(FNavigationSystem::GetCurrent<UNavigationSystemV1>(this)->GetMainNavData());

		if(NavData && BlackboardComponent)
		{
			if(NavData->GetRandomReachablePointInRadius(StartPoint, PatrolRadius, Location, nullptr))
				BlackboardComponent->SetValueAsVector(PatrolLocation.SelectedKeyName, Location);
			else
				BlackboardComponent->SetValueAsVector(PatrolLocation.SelectedKeyName, StartPoint);
			return true;
		}
	}
    return false;
}
