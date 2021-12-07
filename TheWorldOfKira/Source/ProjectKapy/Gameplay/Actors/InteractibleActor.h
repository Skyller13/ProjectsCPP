#include "Components/ArrowComponent.h"
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectKapy/Gameplay/Interfaces/Interactible.h"

#include "InteractibleActor.generated.h"

UCLASS()
class PROJECTKAPY_API AInteractibleActor : public AActor, public IInteractible
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractibleActor();

	UFUNCTION(BlueprintCallable)
	virtual void OnInteractionEnter() override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Interaction_Event();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
