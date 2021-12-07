// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectKapy/Gameplay/Abilities/ShootAbilityBase.h"

#include "CastFX.generated.h"

UCLASS()
class PROJECTKAPY_API ACastFX : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UShootAbilityBase* ShootAbility;
public:
	// Sets default values for this actor's properties
	ACastFX();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetShootAbility(UShootAbilityBase* NewShootAbility);
};
