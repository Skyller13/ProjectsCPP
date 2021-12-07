// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Obstacle_Interface.h"
#include "GameFramework/Actor.h"
#include "Obstacles.generated.h"

UCLASS()
class ANTIVIRUSTACTICAL_API AObstacles : public AActor, public IObstacle_Interface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObstacles();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnObstacleEnterEvent_Implementation() override;
};
