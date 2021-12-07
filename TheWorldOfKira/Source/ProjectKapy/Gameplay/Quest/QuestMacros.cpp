// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestMacros.h"

#include "ProjectKapy/ProjectKapyGameMode.h"
#include "ProjectKapy/Gameplay/Interfaces/Collectible.h"
#include "ProjectKapy/Gameplay/Interfaces/Interactible.h"

QuestMacros::QuestMacros()
{
}

QuestMacros::~QuestMacros()
{
}

AQuest* QuestMacros::CheckForInteraction(AActor* _actor)
{
    // TArray<AActor*> actors;
    //
    // _actor->GetAttachedActors(actors, true);
    //
    // for (int i = 0; i < actors.Num(); i++)
    // {
    //     auto quest = Cast<AQuest>(actors[i]);
    //     
    //     if (quest != nullptr)
    //     {
    //         return quest;
    //     }
    // }
    //
    // if (_actor->Implements<IInteractible>())
    // {
    //     return nullptr;
    // }
    //
    
    return nullptr;
}

void QuestMacros::UpdateUIActiveQuest(UWorld* _world)
{
    if (_world)
    {
        auto mode = Cast<AProjectKapyGameMode>(_world->GetAuthGameMode());

        if (mode)
        {
            
            //TODO mode->MainWidget => objectives => update obj
        }
    }
}

bool QuestMacros::CheckForCollectible(AActor* _item)
{
    // if (_item)
    // {
    //     if (_item->Implements<ICollectible>())
    //     {
    //         return true;   
        // }

        return false;
    // }
}
