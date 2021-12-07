// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectKapy/Gameplay/Abilities/AbilityActor/AbilityActorBase.h"
#include "HitboxAbilityActor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKAPY_API AHitboxAbilityActor : public AAbilityActorBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* HitboxMesh;

	UPROPERTY()
	TArray<AActor*> ActorsHit;

public:
	AHitboxAbilityActor();

protected:
	virtual void BeginPlay() override;


	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnAttackInterrupted();
public:

	virtual void OnAttackInterrupted();
	
	virtual void OnAbilityActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                        const FHitResult& SweepResult) override;
};
