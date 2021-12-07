// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveToTargetActorTaskNode.h"

#include "AIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UMoveToTargetActorTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AAIControllerBase* AIController = Cast<AAIControllerBase>(OwnerComp.GetOwner());

    if (AIController
        && OwnerComp.GetBlackboardComponent())
    {
        AActor* currentTarget = Cast<AActor>(
            OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));
        AActor* currentPawn = AIController->GetPawn();

        if (currentPawn && currentTarget)
        {
            auto result = AIController->MoveToActor(currentTarget, 100.0f);

            if (result == EPathFollowingRequestResult::Failed)
                return EBTNodeResult::InProgress; //continue 
        }
    }

    return EBTNodeResult::Failed; //stop
}