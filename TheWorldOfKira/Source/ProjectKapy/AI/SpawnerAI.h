// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "SpawnerAI.generated.h"

class ADayNightCycleManager;

USTRUCT()
struct FActorSpawnable
{
	GENERATED_BODY()

	FActorSpawnable()
		: ActorClass(UClass::StaticClass()), Actor(nullptr), PercentageDay(0), PercentageNight(0), IsIndestructible(false)
	{}

	FActorSpawnable(TSubclassOf<AActor> actorClass, AActor* actor, int percentageDay, int percentageNight, bool isIndesctructible)
        : ActorClass(actorClass), Actor(actor), PercentageDay(percentageDay), PercentageNight(percentageNight), IsIndestructible(isIndesctructible)
	{}

	UPROPERTY(EditAnywhere, Category = "Setup")
	TSubclassOf<AActor> ActorClass;

	UPROPERTY(VisibleAnywhere, Category = "Setup")
	AActor* Actor;

	UPROPERTY(EditAnywhere, Category = "Setup", meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	int32 PercentageDay;

	UPROPERTY(EditAnywhere, Category = "Setup", meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	int32 PercentageNight;

	UPROPERTY(EditAnywhere, Category = "Setup")
	bool IsIndestructible;

	FORCEINLINE bool operator==(const FActorSpawnable& other) const
	{
		return other.Actor->GetName() == Actor->GetName();
	}
	
};

UCLASS(Blueprintable)
class PROJECTKAPY_API ASpawnerAI : public AActor
{
	GENERATED_BODY()
public:
	ASpawnerAI();
	~ASpawnerAI();

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxOfSpawn;

	UPROPERTY(EditAnywhere, Category = "Setup")
	TArray<FActorSpawnable> SpawnableActors;

	UPROPERTY(VisibleAnywhere)
	TArray<FActorSpawnable> SpawnedActors;

	UPROPERTY(EditAnywhere, Category = "Setup", meta = (ClampMin = "0", ClampMax = "20", UMin = "0", UMax = "20"))
	int32 ActorsMax = 5;

	UPROPERTY(EditAnywhere, Category = "Setup", meta = (ClampMin = "0.0", UMin = "0.0"))
	float TimeBetweenEachSpawn = 60.0f;

	UPROPERTY(VisibleAnywhere)
	float TimerSpawn = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Setup", meta = (ClampMin = "0", ClampMax = "20", UMin = "0", UMax = "20"))
	int32 SpawnBeginPlayMax = 5;

	UPROPERTY(EditAnywhere, Category = "Setup", meta = (ClampMin = "0", ClampMax = "20", UMin = "0", UMax = "20"))
	int32 SpawnBeginPlayMin = 1;

	UPROPERTY(EditAnywhere, Category = "Setup", meta = (ClampMin = "1", UMin = "1"))
	float DistanceMaxRespawn = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Setup", meta = (ClampMin = "1", ClampMax = "20", UMin = "1", UMax = "20"))
	int32 NumberMaxRespawn = 1;

	UPROPERTY(EditAnywhere, Category = "Setup")
	bool RandomRespawn = false;

protected:
	virtual void BeginPlay() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void EmptySpawn(bool EvenIndestructible);

	UFUNCTION()
	void RemoveActor(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> SpawnQuest(TMap<TSubclassOf<AActor>, int32> ListActors);

private:
	UPROPERTY()
	ADayNightCycleManager* DayNightManager;
};
