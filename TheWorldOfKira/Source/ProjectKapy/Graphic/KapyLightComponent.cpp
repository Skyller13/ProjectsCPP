// Fill out your copyright notice in the Description page of Project Settings.


#include "KapyLightComponent.h"
#include "DayNightCycleManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UKapyLightComponent::UKapyLightComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UKapyLightComponent::BeginPlay()
{
	Super::BeginPlay();

	DayNightCycleManager = Cast<ADayNightCycleManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADayNightCycleManager::StaticClass()));

	if (DayNightCycleManager)
	{
		DayNightCycleManager->PointSpotLightRegister(GetOwner(), this);
	}
	
}


// Called every frame
void UKapyLightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

