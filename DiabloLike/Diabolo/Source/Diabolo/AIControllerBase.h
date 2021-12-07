// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "AIControllerBase.generated.h"

UCLASS()
class DIABOLO_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public :

	UPROPERTY(EditAnywhere, Category = "Controller")
	UBlackboardData* BBAsset;

	UPROPERTY(EditAnywhere, Category = "Controller")
	UBehaviorTree* BTAsset;

	void InitBT();
};
