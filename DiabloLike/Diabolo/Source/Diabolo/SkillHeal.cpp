// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillHeal.h"

#include "AICharacter.h"
#include "SkillManagerComponent.h"
#include "HealthComponent.h"

void USkillHeal::Execute()
{
    Super::Execute();

    count = 0;
    currentHealTimer = 0.f;

    if (!isOnTime)
    {
        if (SkillManagerComponent)
        {
            auto* player = Cast<ADiaboloCharacter>(SkillManagerComponent->GetOwnerCharacter());

            if (player && healParticles)
            {
                player->HealthComponent->AddHealth(healValue);

                healActorRef = GetWorld()->SpawnActor<AActor>(healParticles, player->GetActorLocation(),
                                                              player->GetActorRotation());
                healActorRef->AttachToActor(player, FAttachmentTransformRules::KeepWorldTransform);
            }
            else
            {
                auto* AICharacter = Cast<AAICharacter>(SkillManagerComponent->GetOwnerCharacter());

                if (AICharacter && healParticles)
                {
                    AICharacter->HealthComponent->AddHealth(healValue);

                    healActorRef = GetWorld()->SpawnActor<AActor>(healParticles, AICharacter->GetActorLocation(),
                                                                  AICharacter->GetActorRotation());
                    healActorRef->AttachToActor(AICharacter, FAttachmentTransformRules::KeepWorldTransform);
                }
            }
        }
    }
    else
    {
        healTickTime = healingTime / nbHealTick;
        CreateHeal();
    }
}

void USkillHeal::CreateHeal()
{
    if (healActorRef == nullptr && healParticles)
    {
        auto* player = Cast<ADiaboloCharacter>(SkillManagerComponent->GetOwnerCharacter());

        if (player)
        {
            healActorRef = GetWorld()->SpawnActor<AActor>(healParticles, player->GetActorLocation(),
                                              player->GetActorRotation());
            healActorRef->AttachToActor(player, FAttachmentTransformRules::KeepWorldTransform);
        }


        auto* AICharacter = Cast<AAICharacter>(SkillManagerComponent->GetOwnerCharacter());

        if (AICharacter)
        {
            healActorRef = GetWorld()->SpawnActor<AActor>(healParticles, AICharacter->GetActorLocation(),
                                                          AICharacter->GetActorRotation());
            healActorRef->AttachToActor(AICharacter, FAttachmentTransformRules::KeepWorldTransform);
        }
    }
}


void USkillHeal::TickSkill(float _DeltaTime)
{
    Super::TickSkill(_DeltaTime);

    if (isOnTime
        && healActorRef
        && count < nbHealTick)
    {
        currentHealTimer += _DeltaTime;

        if (currentHealTimer >= healTickTime)
        {
            currentHealTimer = 0.0f;

            if (SkillManagerComponent)
            {
                //set heal on time
                auto* player = Cast<ADiaboloCharacter>(SkillManagerComponent->GetOwnerCharacter());

                if (player && healActorRef)
                {
                    player->HealthComponent->AddHealth(healValue / nbHealTick);
                }
                else
                {
                    auto* AICharacter = Cast<AAICharacter>(SkillManagerComponent->GetOwnerCharacter());

                    if (AICharacter && healActorRef)
                    {
                        AICharacter->HealthComponent->AddHealth(healValue / nbHealTick);
                    }
                }
            }

            count++;

            if (count >= nbHealTick)
            {
                healActorRef->Destroy();
                healActorRef = nullptr;
            }
        }
    }
    else
    {
        if (healActorRef != nullptr
            && !isOnTime)
        {
            currentHealTimer += _DeltaTime;

            if (currentHealTimer > 2.f)
            {
                healActorRef->Destroy();
                healActorRef = nullptr;
            }
        }
    }
}
