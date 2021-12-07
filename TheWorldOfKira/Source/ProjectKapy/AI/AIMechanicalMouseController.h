// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIControllerBase.h"
#include "AIMechanicalMouseController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class PROJECTKAPY_API AAIMechanicalMouseController : public AAIControllerBase
{
	GENERATED_BODY()
	
protected:
	AAIMechanicalMouseController();

	UPROPERTY(BlueprintReadOnly)
	FVector EndPoint;

	UPROPERTY(BlueprintReadOnly)
	TArray<FVector> FollowPoints;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentPoint = 0;

	UPROPERTY()
	float TimerMoveTo = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Setup")
	float MaxRadiusMovement = 150.0f;
	UPROPERTY(EditAnywhere, Category = "Setup")
	float MaxRadiusPatrol = 1000.0f;

	virtual void BeginPlay() override;

    virtual void Tick(float DeltaSeconds) override;

public:
	UFUNCTION(BlueprintCallable)
	bool CreateFollowPoints();

	UFUNCTION(BlueprintCallable)
    void GoToNextPoint(float DeltaSeconds);
	
	UFUNCTION(BlueprintCallable)
	bool MoveToFollowPoints(float DeltaSeconds);

	UFUNCTION()
	TArray<FVector> GetFollowPoints() const { return FollowPoints; };

	UFUNCTION()
	int32 GetCurrentPoint() const { return CurrentPoint; };

	UFUNCTION()
	FVector GetEndPoint() const { return EndPoint; };
};
