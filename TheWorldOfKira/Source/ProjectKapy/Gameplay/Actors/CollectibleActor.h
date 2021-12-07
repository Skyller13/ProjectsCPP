// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectKapy/Gameplay/Item/CollectibleData.h"
#include "ProjectKapy/Gameplay/Interfaces/Collectible.h"

#include "CollectibleActor.generated.h"

UCLASS()
class PROJECTKAPY_API ACollectibleActor : public AActor, public ICollectible
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectibleActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCollectibleData data;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void OnCollect() override;

	UFUNCTION(BlueprintCallable)
	virtual void ActivateCollectible();

	UFUNCTION(BlueprintCallable)
	virtual void DeactivateCollectible();
};