// Fill out your copyright notice in the Description page of Project Settings.

#include "AIControllerBase.h"

#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AIPerceptionTypes.h"
#include "ProjectKapy/NPCCharacter.h"
#include "ProjectKapy/Gameplay/Combat/LifeSystemComponent.h"

AAIControllerBase::AAIControllerBase()
{
}

void AAIControllerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(LastActor != nullptr)
	{
		ANPCCharacter* LastCharacter = Cast<ANPCCharacter>(LastActor);
		if(LastCharacter != nullptr && LastCharacter->GetLifeSystem()->HpCurrent <= 0.0f)
		{
			BlackboardComponent->ClearValue("HasLineOfSight");
			BlackboardComponent->ClearValue("PlayerActor");
			LastActor = nullptr;
			IsNPCLineOfSight = false;
		}
	}
}

void AAIControllerBase::HasStimulus(AActor* Actor, FAIStimulus Stimulus)
{
    BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(this);
	
	if(BlackboardComponent)
	{
		if(!Stimulus.Type.Name.Compare("Default__AISense_Sight"))
		{
			if(!Stimulus.IsActive() && Actor && Actor->ActorHasTag("Player") && LastActor && !IsNPCLineOfSight)
				LastLineOfSight(Actor);
			else if (Actor && Actor->ActorHasTag("NPC"))
				SetLineOfSight(Actor, true);
			else if (Actor && Actor->ActorHasTag("Player") && !IsNPCLineOfSight)
				SetLineOfSight(Actor);
		}
		else if(!Stimulus.Type.Name.Compare("Default__AISense_Hearing"))
		{
			if(Stimulus.IsActive() && BlackboardComponent->GetValueAsObject("PlayerActor") == nullptr && Actor)
				SetHearing(Actor);
		}
	}
}

void AAIControllerBase::SetLineOfSight(AActor* Actor, bool IsNPC)
{
	if(!BlackboardComponent)
		BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(this);

	if(BlackboardComponent)
	{
		LastActor = Actor;
		BlackboardComponent->SetValueAsBool("HasLineOfSight", true);
		BlackboardComponent->SetValueAsObject("PlayerActor", Actor);
		BlackboardComponent->ClearValue("LoseLineOfSight");
		BlackboardComponent->ClearValue("LastPlayerLocation");
		GetPawn()->GetWorldTimerManager().ClearTimer(TimerHandleLineOfSight);
		UpdatePeripheralVisionAngleDegrees(180.0f);

		IsNPCLineOfSight = IsNPC;
	}
}

void AAIControllerBase::LastLineOfSight(AActor* Actor)
{
	if(!BlackboardComponent)
		BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(this);

	if(BlackboardComponent)
	{
		LastActor = nullptr;
		LastPlayerLocation = Actor->GetActorLocation();
		BlackboardComponent->SetValueAsBool("LoseLineOfSight", true);
		BlackboardComponent->SetValueAsVector("LastPlayerLocation", LastPlayerLocation);
		BlackboardComponent->ClearValue("HasLineOfSight");
		BlackboardComponent->ClearValue("PlayerActor");
		UpdatePeripheralVisionAngleDegrees(90.0f);
	}
}

void AAIControllerBase::SetHearing(AActor* Actor)
{
	if(!BlackboardComponent)
		BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(this);

	if(BlackboardComponent)
	{
		LastActor = nullptr;
		BlackboardComponent->SetValueAsBool("HasHearingMouseMecha", true);
		BlackboardComponent->SetValueAsObject("HearedMouseMecha", Actor);
		BlackboardComponent->ClearValue("LoseLineOfSight");
		BlackboardComponent->ClearValue("LastPlayerLocation");
	}
}

void AAIControllerBase::HasDamageFromPlayer()
{	
	if(!BlackboardComponent)
		BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(this);

	AActor* Player = UGameplayStatics::GetPlayerController(this, 0)->GetPawn();
	if(Player && FVector::Distance(Player->GetActorLocation(), this->GetPawn()->GetActorLocation()) < 3000.0f)
		SetLineOfSight(Player);
}

void AAIControllerBase::UpdatePeripheralVisionAngleDegrees(float Degree)
{
	UAIPerceptionComponent* PerceptionComp = GetAIPerceptionComponent();
	if (PerceptionComp)
	{
		FAISenseID SenseID = UAISense::GetSenseID<UAISense_Sight>();
		UAISenseConfig_Sight* SenseConfig = Cast<UAISenseConfig_Sight>(PerceptionComp->GetSenseConfig(SenseID));
		if (SenseConfig)
		{
			SenseConfig->PeripheralVisionAngleDegrees = Degree;
			PerceptionComp->ConfigureSense(*SenseConfig);
		}
	}
}
