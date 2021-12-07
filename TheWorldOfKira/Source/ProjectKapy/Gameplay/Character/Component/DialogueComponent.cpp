// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueComponent.h"

#include "Kismet/GameplayStatics.h"
#include "ProjectKapy/NPCCharacter.h"

// Sets default values for this component's properties
UDialogueComponent::UDialogueComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}


// Called when the game starts
void UDialogueComponent::BeginPlay()
{
    Super::BeginPlay();

    // ...
    OnDialogueStartEvent.AddUniqueDynamic(this, &UDialogueComponent::TalkToTarget);
}

void UDialogueComponent::TalkToTarget()
{
    if (DialogueTable)
    {
        if (Target && isInDialogue)
        {
            auto* Dialogue = Cast<UDialogueComponent>(Target->GetComponentByClass(StaticClass()));

            if (Dialogue)
            {
                auto* Owner = Cast<ANPCCharacter>(GetOwner());

                if (Owner && Voice && DialogueOverhaulStructs.Num() > 0)
                {
                    //talk to the target
                    Owner->TalkToTarget_Event(Target);
                }
            }
        }
    }
}

// Called every frame
void UDialogueComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}
