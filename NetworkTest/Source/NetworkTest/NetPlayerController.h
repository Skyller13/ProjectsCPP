// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NetPlayerController.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum ETeamName
{
	TeamA,
	TeamB,
};

UCLASS()
class NETWORKTEST_API ANetPlayerController : public APlayerController
{
	GENERATED_BODY()

public:	
	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<ETeamName> TeamName;
};
