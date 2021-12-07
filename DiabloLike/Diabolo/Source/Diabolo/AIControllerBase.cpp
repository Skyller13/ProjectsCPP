// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"

void AAIControllerBase::BeginPlay()
{
    Super::BeginPlay();

    // FTimerHandle InitTimer;
    //
    // GetWorldTimerManager().SetTimer(InitTimer, this, &AAIControllerBase::InitBT, 0.2f,
    //                                         false);
}

void AAIControllerBase::InitBT()
{
    if (BTAsset)
        RunBehaviorTree(BTAsset);

    UBlackboardComponent* currentBB;

    if (BBAsset)
        UseBlackboard(BBAsset, currentBB);
}
