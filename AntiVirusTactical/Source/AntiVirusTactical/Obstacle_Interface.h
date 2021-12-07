// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Obstacle_Interface.generated.h"

UINTERFACE(BlueprintType)
class ANTIVIRUSTACTICAL_API UObstacle_Interface : public UInterface
{
	GENERATED_BODY()
	
public:
	UObstacle_Interface();
	~UObstacle_Interface();
};

class ANTIVIRUSTACTICAL_API IObstacle_Interface
{
	GENERATED_BODY()
	
	public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Broadcast")
    void OnObstacleEnterEvent();

	virtual void OnObstacleEnterEvent_Implementation();
};