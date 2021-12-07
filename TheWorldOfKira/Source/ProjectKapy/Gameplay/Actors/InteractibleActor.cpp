// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractibleActor.h"


#include "Kismet/GameplayStatics.h"
#include "ProjectKapy/KiraCharacter.h"
#include "ProjectKapy/ProjectKapyCharacter.h"
#include "ProjectKapy/Gameplay/Character/Component/QuestLogComponent.h"
#include "ProjectKapy/Gameplay/Quest/Quest.h"

// Sets default values
AInteractibleActor::AInteractibleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AInteractibleActor::OnInteractionEnter()
{
	auto player = Cast<AKiraCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (player)
	{
		auto quests = player->QuestLog->Quests;

		for (auto Element : quests)
		{
			Element->CheckInteractionObjective(this);
		}
	}
	
	Interaction_Event();
}

// Called when the game starts or when spawned
void AInteractibleActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AInteractibleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}