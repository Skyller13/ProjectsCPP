// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilityActorBase.generated.h"

class UAbilityBase;


UCLASS()
class PROJECTKAPY_API AAbilityActorBase : public AActor
{
	GENERATED_BODY()


protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	UAbilityBase* AbilityOwner;

	float ElapsedTimeSinceSpawn = 0.f;

public:
	AAbilityActorBase();

protected:
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable)
	virtual void Init(UAbilityBase* Ability);
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnAbilityActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                        const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnAbilityActorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void OnAbilityActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	                               UPrimitiveComponent* OtherComp,
	                               FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnAbilityActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                   const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnAbilityActorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnAbilityActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                          FVector NormalImpulse, const FHitResult& Hit);
};
