// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillGrenade.h"


#include "AICharacter.h"
#include "DiaboloCharacter.h"
#include "SkillManagerComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

void USkillGrenade::Execute()
{
    ThrowGrenade();

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

void USkillGrenade::TickSkill(float _DeltaTime)
{
    Super::TickSkill(_DeltaTime);

    if (isGrenadeThrew)
    {
        if (elapsedTime < timerUntilExplosion)
        {
            elapsedTime += _DeltaTime;
        }
        else
        {
            //can Explode
            ExplosionGrenade();
        }
    }
}

void USkillGrenade::ExplosionGrenade()
{
    Event_OnExplosionEvent();

    isGrenadeThrew = false;
    elapsedTime = 0.f;

    FTimerHandle destroyTimer;

    SkillManagerComponent->GetOwnerCharacter()->GetWorldTimerManager().SetTimer(
         destroyTimer, this, &USkillGrenade::DestroyGrenade, 0.3f,
         false);
}

void USkillGrenade::ThrowGrenade()
{
    //Throw grenade in the direction of the mouse
    Event_ThrowGrenade();

    if (SkillManagerComponent)
    {
        grenadeRef = GetWorld()->SpawnActor<AActor>(grenadeObj,
                                                    SkillManagerComponent->GetOwnerCharacter()->GetMesh()->
                                                                           GetSocketLocation("Palm_L"),
                                                    SkillManagerComponent->GetOwnerCharacter()->GetMesh()->
                                                                           GetSocketRotation("Palm_L"));

        grenadeRef->AttachToComponent(SkillManagerComponent->GetOwnerCharacter()->GetMesh(),
                                      FAttachmentTransformRules::KeepWorldTransform, "Palm_L");

        ImpulseDirection = SkillManagerComponent->GetOwnerCharacter()->GetActorForwardVector().GetSafeNormal();
    }
}

void USkillGrenade::DestroyGrenade()
{
    if (grenadeRef)
    {
        grenadeRef->Destroy();
        grenadeRef = nullptr;
    }
}

void USkillGrenade::SkillBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& SweepResult)
{
    Super::SkillBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}
