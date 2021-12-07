// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "ProjectKapy/ProjectKapyCharacter.h"

#include "KillEnemiesArea.generated.h"

class AQuest;
UCLASS()
class PROJECTKAPY_API AKillEnemiesArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKillEnemiesArea();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* Box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AProjectKapyCharacter*> EnemiesArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TSubclassOf<AActor>, int32> EnemiesToSpawn;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere)
	AQuest* QuestRef = nullptr;

	UFUNCTION()
	void SpawnEnemiesAtLocation();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void CheckQuestArea();

	UFUNCTION()
	void SetQuestReference(AQuest* _ref);

	UFUNCTION()
	void LinkSpawnTriggerEvent();
};