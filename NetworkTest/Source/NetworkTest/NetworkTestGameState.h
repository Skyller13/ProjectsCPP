// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"

#include "NetworkTestGameState.generated.h"

UCLASS()
class NETWORKTEST_API ANetworkTestGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	int32 TeamScoreA;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	int32 TeamScoreB;
};
