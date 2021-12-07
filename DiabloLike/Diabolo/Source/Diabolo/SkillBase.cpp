// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillBase.h"

#include "AICharacter.h"
#include "SkillManagerComponent.h"
#include "SkillActor.h"

void USkillBase::Execute()
{
    isExecuting = true;
    currentCastTime = -1.0f;

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
    
    EndExecution();
}

void USkillBase::EndExecution()
{
    cooldownTimer = cooldown;

    Event_EndExecution();

    isExecuting = false;
}

bool USkillBase::RequestExecution()
{
    if (cooldownTimer <= 0.0f)
    {
        if (castTime <= 0.0f)
            Execute();
        else
        {
            isExecuting = true;
            currentCastTime = castTime;
        }

        return true;
    }

    return false;
}

void USkillBase::TickSkill(float _DeltaTime)
{
    if (cooldownTimer > 0.0f)
    {
        cooldownTimer -= GetWorld()->DeltaTimeSeconds;        
    }

    if (cooldownTimer < 0.0f)
    {
        cooldownTimer = 0.0f;
    }

    if (currentCastTime > 0.0f)
    {
        currentCastTime -= _DeltaTime;

        if (currentCastTime <= 0.0f)
            Execute();
    }

    Event_TickSkill(_DeltaTime);
}

void USkillBase::SkillBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Event_SkillBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void USkillBase::SkillEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    Event_SkillEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
