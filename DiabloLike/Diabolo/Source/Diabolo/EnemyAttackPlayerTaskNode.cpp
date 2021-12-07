// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAttackPlayerTaskNode.h"


#include "AICharacter.h"
#include "AIControllerBase.h"
#include "SkillManagerComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UEnemyAttackPlayerTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AAIControllerBase* AIController = Cast<AAIControllerBase>(OwnerComp.GetOwner());

    if (AIController
      && OwnerComp.GetBlackboardComponent())
    {
        if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(boolKey.SelectedKeyName))
        {
            //Attack player if he is near the AI
            auto* AICharacter = Cast<AAICharacter>(AIController->GetPawn());

            if (AICharacter
                && AICharacter->SkillManagerComponent)
            {
                if (!AICharacter->SkillManagerComponent->IsASkillExecuting())
                {
                    AICharacter->SkillManagerComponent->OnSkillRequested(0);
                    return EBTNodeResult::Succeeded; 
                }
            } 
        }
    }

    return EBTNodeResult::Failed; 
}
