// Fill out your copyright notice in the Description page of Project Settings.


#include "CastFX.h"

// Sets default values
ACastFX::ACastFX()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACastFX::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACastFX::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACastFX::SetShootAbility(UShootAbilityBase* NewShootAbility)
{
	ShootAbility = NewShootAbility;
}

