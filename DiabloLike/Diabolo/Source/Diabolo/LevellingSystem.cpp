// Fill out your copyright notice in the Description page of Project Settings.


#include "LevellingSystem.h"

#include "DiaboloCharacter.h"
#include "DiaboloGameMode.h"
#include "DiaboloPlayerController.h"
#include "HealthComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
ULevellingSystem::ULevellingSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULevellingSystem::BeginPlay()
{
	Super::BeginPlay();

	// ...

	//Create widget
	if (!ExpUI)
	{
		auto characterController = Cast<ADiaboloPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

		if (characterController)
		{
			ExpUI = characterController->ExperienceUI;
		}
	}
	
	CalculatePercentage();
}


// Called every frame
void ULevellingSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	CurrentExperience = ceil(FMath::FInterpTo(CurrentExperience, BufferExperience, DeltaTime, 5.0f));

	CalculatePercentage();

	if (CurrentExperience >= CurrentMaxExperience)
	{
		LevelUp();

		auto characterController = Cast<ADiaboloPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

		if (characterController)
		{
			//Add level up widget here !
		}
	}
}

void ULevellingSystem::CalculatePercentage()
{
	CurrentPercentage = CurrentExperience / CurrentMaxExperience;
}

void ULevellingSystem::CalculateMaxExperience(float _factorByLevel)
{
	CurrentMaxExperience = ceil(CurrentMaxExperience * _factorByLevel);
}

void ULevellingSystem::AddExperience(float _value)
{
	ExpUI->SetVisibility(ESlateVisibility::Visible);
	ExpUI->SetColorAndOpacity(FColor::White);
	BufferExperience += _value;
}

void ULevellingSystem::LevelUp()
{ 
	if (CurrentLevel < LevelCap)
	{
		CurrentLevel++;

		auto character = Cast<ADiaboloCharacter>(GetOwner());
		
		if (character)
		{
			//add stats health
			character->HealthComponent->HealthMax += (character->HealthComponent->HealthMax * 0.075f);
			character->HealthComponent->BufferHealth = character->HealthComponent->HealthMax;

			//add stats vel
			character->maxWalkSpeed += (character->maxWalkSpeed* 0.03f);
		}
		
		float RemainingExperience = BufferExperience - CurrentMaxExperience;
		CalculateMaxExperience(1.7f);
		CurrentExperience = 0.0f;
		BufferExperience = 0.0f;
		AddExperience(RemainingExperience);
	}
	else
	{
		CurrentExperience = CurrentMaxExperience;
		BufferExperience = CurrentMaxExperience;
	}
}
