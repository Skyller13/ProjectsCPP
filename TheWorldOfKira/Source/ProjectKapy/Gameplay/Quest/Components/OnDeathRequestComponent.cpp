// Fill out your copyright notice in the Description page of Project Settings.


#include "OnDeathRequestComponent.h"


#include "Kismet/GameplayStatics.h"
#include "ProjectKapy/KiraCharacter.h"
#include "ProjectKapy/Gameplay/Character/Component/EnemyStatComponent.h"
#include "ProjectKapy/Gameplay/Character/Component/LevelSystemComponent.h"
#include "ProjectKapy/Gameplay/Character/Component/QuestLogComponent.h"
#include "ProjectKapy/Gameplay/Combat/LifeSystemComponent.h"
#include "ProjectKapy/Gameplay/Quest/Quest.h"

// Sets default values for this component's properties
UOnDeathRequestComponent::UOnDeathRequestComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}


// Called when the game starts
void UOnDeathRequestComponent::BeginPlay()
{
    Super::BeginPlay();

    // ...

    ownerCharacter = Cast<AProjectKapyCharacter>(GetOwner());

    if (ownerCharacter)
    {
        ownerCharacter->LifeSystem->OnDeath.AddUniqueDynamic(this, &UOnDeathRequestComponent::OnDeathRequest);
    }
}


// Called every frame
void UOnDeathRequestComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

void UOnDeathRequestComponent::OnDeathRequest()
{
    const auto Owner = GetOwner();

    if (ownerCharacter)
    {
        auto player = Cast<AKiraCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

        if (player)
        {
            if (player->GetLevelSystemComp() != nullptr)
            {
                const auto* enemy = Cast<AProjectKapyCharacter>(GetOwner());

                if (enemy)
                {
                    const auto* stats = Cast<UEnemyStatComponent>(
                        enemy->GetComponentByClass(UEnemyStatComponent::StaticClass()));

                    if (stats)
                    {
                        //add experience to player and display it !
                        
                        const auto* dayManager = Cast<ADayNightCycleManager>(
                            UGameplayStatics::GetActorOfClass(GetWorld(), ADayNightCycleManager::StaticClass()));

                        if (dayManager)
                            if (dayManager->DayNightMoment == NIGHT)
                                player->GetLevelSystemComp()->AddExperience(stats->experienceReward * 1.2f, true);
                            else
                                player->GetLevelSystemComp()->AddExperience(stats->experienceReward, true);
                    }
                }
            }

            auto quests = player->QuestLog->Quests;

            for (auto Element : quests)
            {
                Element->CheckKilledEnemy(Owner);
            }

            DestroyComponent();
        }
    }
}
