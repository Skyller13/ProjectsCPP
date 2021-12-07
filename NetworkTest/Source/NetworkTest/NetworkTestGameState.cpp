// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkTestGameState.h"
#include "Net/UnrealNetwork.h"

void ANetworkTestGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ANetworkTestGameState, TeamScoreA);
    DOREPLIFETIME(ANetworkTestGameState, TeamScoreB);
}
