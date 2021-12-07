// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillAOE.h"

#include "AICharacter.h"
#include "SkillManagerComponent.h"

void USkillAOE::Execute()
{    
    isExecuting = true;
    currentCastTime = -1.0f;

    SpawnDetectionMesh();

    if (SkillManagerComponent)
    {
        auto* player = Cast<ADiaboloCharacter>(SkillManagerComponent->GetOwnerCharacter());

        if (player)
        {
            player->SetCurrentSkill(this);
        }
        else
        {
            auto* AICharacter = Cast<AAICharacter>(SkillManagerComponent->GetOwnerCharacter());

            if (AICharacter)
            {
                AICharacter->SetCurrentSkill(this);
            }
        }
    }

    Event_Execute();

    FTimerHandle ExecuteTimerHandle;
    SkillManagerComponent->GetOwnerCharacter()->GetWorldTimerManager().SetTimer(ExecuteTimerHandle, this, &USkillAOE::ExecuteHandle, 0.15f);
}

void USkillAOE::ExecuteHandle()
{
    Super::EndExecution();
}

void USkillAOE::SkillBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::SkillBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

    //damage here
}

