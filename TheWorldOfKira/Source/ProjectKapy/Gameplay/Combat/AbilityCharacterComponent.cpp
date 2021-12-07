// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityCharacterComponent.h"
#include "ProjectKapy/ProjectKapyCharacter.h"
// Sets default values for this component's properties
UAbilityCharacterComponent::UAbilityCharacterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAbilityCharacterComponent::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<AProjectKapyCharacter>(GetOwner());
}


// Called every frame
void UAbilityCharacterComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bIsPress)
	{
		PressElapsedTime += DeltaTime;
	}
	// ...
}

void UAbilityCharacterComponent::AbilityPress()
{
	if (!bIsPress)
	{
		bIsPress = true;
		PressElapsedTime = 0.f;
	}
}

void UAbilityCharacterComponent::AbilityRelease()
{
	if (bIsPress)
		bIsPress = false;
}

void UAbilityCharacterComponent::Interrupt()
{
	if (bIsPress)
		bIsPress = false;
}

// bool UAbilityCharacterComponent::ValidState(ECharacterState newState)
// {
// 	return true;
// }
//
// void UAbilityCharacterComponent::ResetState(ECharacterState previousState)
// {
// }
//
// void UAbilityCharacterComponent::InitNewState(ECharacterState newState)
// {
// }
