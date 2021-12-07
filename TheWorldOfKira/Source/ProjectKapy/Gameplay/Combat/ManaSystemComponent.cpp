// Fill out your copyright notice in the Description page of Project Settings.


#include "ManaSystemComponent.h"

// Sets default values for this component's properties
UManaSystemComponent::UManaSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UManaSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	ManaCurrent = ManaBase;
	// ...
}


// Called every frame
void UManaSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ManaCurrent < ManaBase)
		ManaCurrent += ManaRegerationRate * DeltaTime;

	ManaCurrent = FMath::Clamp(ManaCurrent, 0.f, ManaBase);

	// ...
}

void UManaSystemComponent::AddMana(float _mana)
{
	ManaCurrent += _mana;
	ManaCurrent = FMath::Clamp(ManaCurrent, 0.f, ManaBase);
}

void UManaSystemComponent::ConsumeMana(float _mana)
{
	ManaCurrent -= _mana;
	ManaCurrent = FMath::Clamp(ManaCurrent, 0.f, ManaBase);
}

float UManaSystemComponent::GetManaPercent()
{
	if (ManaBase != 0.f)
		return ManaCurrent / ManaBase;

	return 0.f;
}

bool UManaSystemComponent::HaveEnoughtMana(float _manaCost)
{
	return ManaCurrent >= _manaCost;
}
