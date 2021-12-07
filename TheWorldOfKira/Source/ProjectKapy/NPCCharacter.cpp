// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCCharacter.h"


#include "AIController.h"
#include "KiraCharacter.h"
#include "ProjectKapyCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Gameplay/Combat/LifeSystemComponent.h"
#include "Gameplay/Character/Component/DialogueComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameplay/Character/Component/LevelSystemComponent.h"
#include "Gameplay/Character/Component/QuestLogComponent.h"
#include "Gameplay/Quest/Quest.h"
#include "Graphic/ToonShaderPointLightDetector.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ANPCCharacter::ANPCCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    //Initialize components
    dialogueTriggerArea = CreateDefaultSubobject<UBoxComponent>(TEXT("DialogueTrigger"));
    QuestMarkerLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("QuestMarkerLocation"));
    DialogueComponent = CreateDefaultSubobject<UDialogueComponent>(TEXT("DialogueComponent"));
    LifeSystemComponent = CreateDefaultSubobject<ULifeSystemComponent>(TEXT("LifeSystemComponent"));

    ShaderComponent = CreateDefaultSubobject<UToonShaderPointLightDetector>(TEXT("ToonShaderComponent"));

    QuestMarkerLocation->SetupAttachment(dialogueTriggerArea);

    //Set position, scale, rotation
    dialogueTriggerArea->SetRelativeScale3D(FVector(4.75f, 4.75f, 2.75f));
    QuestMarkerLocation->SetRelativeLocation(FVector(0.f, 0.f, 25.5f));
    QuestMarkerLocation->ArrowSize = 0.1f;
}

void ANPCCharacter::OnInteractionEnter()
{
    auto character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    if (character)
    {
        auto Kira = Cast<AKiraCharacter>(character);

        if (Kira != nullptr)
        {
            auto rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),
                                                                   Kira->GetActorLocation());

            SetActorRotation(FRotator(0.f, rotation.Yaw, 0.f));

            auto quests = Kira->QuestLog->Quests;

            for (auto Element : quests)
            {
                Element->CheckInteractionObjective(this);
            }
        }
    }
}

// Called when the game starts or when spawned
void ANPCCharacter::BeginPlay()
{
    Super::BeginPlay();

    dialogueTriggerArea->OnComponentBeginOverlap.AddUniqueDynamic(this, &ANPCCharacter::OnDialogueAreaOverlap);
    dialogueTriggerArea->OnComponentEndOverlap.AddUniqueDynamic(this, &ANPCCharacter::OnDialogueAreaEndOverlap);
}

// Called every frame
void ANPCCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //WalkToDestination(FinalLocation);

    CheckPlayerListenToConversation(DeltaTime);

    DisplayQuestWaypoint_Event();
}

// Called to bind functionality to input
void ANPCCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ANPCCharacter::WalkToDestination(FVector _location)
{
    if (bHaveToMove)
    {
        FVector destination = _location;

        auto controller = Cast<AAIController>(GetController());

        if (controller)
        {
            auto playerCharacter = Cast<AProjectKapyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

            if (playerCharacter)
            {
                float dist = FVector::Distance(GetActorLocation(), playerCharacter->GetActorLocation());

                if (dist <= 800.0f)
                {
                    //move to destination
                    GetCharacterMovement()->MaxWalkSpeed = 215.0f;

                    controller->MoveToLocation(destination, 150.0f);
                }
                else
                {
                    if (dist >= 1300.0f)
                    {
                        //player is too far from npc
                        auto rotation = UKismetMathLibrary::FindLookAtRotation(
                            GetActorLocation(), playerCharacter->GetActorLocation());

                        SetActorRotation(UKismetMathLibrary::RLerp(GetActorRotation(), rotation, 0.025f, true));
                        controller->StopMovement();
                    }
                    else
                    {
                        //Not that far
                        controller->StopMovement();
                    }
                }
            }
        }
    }
}

void ANPCCharacter::CheckPlayerListenToConversation(float _DeltaTime)
{
    if (DialogueComponent->isInDialogue)
    {
        auto character = Cast<AKiraCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

        if (character)
        {
            float dist = FVector::Distance(character->GetActorLocation(), GetActorLocation());

            if (dist <= character->RangeToHearConversation)
            {
                auto quests = character->QuestLog->Quests;

                for (auto Element : quests)
                {
                    Element->CheckIsPlayerListening(this, _DeltaTime);
                }
            }
        }
    }
}

void ANPCCharacter::OnDialogueAreaOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                          const FHitResult& SweepResult)
{
    if (DialogueComponent && OtherActor != this)
    {
        if (DialogueComponent->isInDialogue || DialogueComponent->Target != nullptr || !isAbleToTalk)
            return;

        auto* Player = Cast<AKiraCharacter>(OtherActor);
        auto* Npc = Cast<ANPCCharacter>(OtherActor);

        //Can't evaluate distance with Player (there is no available reference)
        if (!Player || !IsValid(Player))
        {
            return;
        }

        const FVector PlayerPos = Player->GetActorLocation();

        const float distance = FVector::Dist(GetActorLocation(), PlayerPos);

        //There is no need to be in dialogue (Player is too far from the NPC)
        if (distance > 750.f)
        {
            return;
        }

        if (Player)
        {
            auto* Dialogue = Cast<UDialogueComponent>(Player->GetComponentByClass(UDialogueComponent::StaticClass()));

            if (Dialogue
                && !Dialogue->isInDialogue)
            {
                if (DialogueComponent->DialogueOverhaulStructs.Contains("Kira"))
                {
                    //Talk to Kira
                    DialogueComponent->Target = Player;
                    DialogueComponent->isInDialogue = true;

                    Dialogue->isInDialogue = true;
                    Dialogue->Target = this;

                    DialogueComponent->OnDialogueStartEvent.Broadcast();
                }
            }
        }
        else if (Npc)
        {
            auto* Dialogue = Cast<UDialogueComponent>(Npc->GetComponentByClass(UDialogueComponent::StaticClass()));

            if (Dialogue
                && !Dialogue->isInDialogue
                && !Dialogue->Target)
            {
                if (DialogueComponent->DialogueOverhaulStructs.Contains(Npc->GetName()))
                {
                    //Talk with another NPC
                    DialogueComponent->Target = Npc;
                    DialogueComponent->isInDialogue = true;

                    Dialogue->Target = this;
                    Dialogue->isInDialogue = true;

                    DialogueComponent->OnDialogueStartEvent.Broadcast();
                }
            }
        }
    }
}

void ANPCCharacter::OnDialogueAreaEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (DialogueComponent && OtherActor != this)
    {
        auto* Player = Cast<AKiraCharacter>(OtherActor);

        if (Player)
        {
            if (DialogueComponent->Target == Player)
            {
                auto* Dialogue = Cast<UDialogueComponent>(
                    Player->GetComponentByClass(UDialogueComponent::StaticClass()));

                if (Dialogue)
                {
                    FTimerDelegate TimerDelegate;
                    FTimerHandle DialogueHandle;

                    TimerDelegate.BindUFunction(this, FName("EndDialogue"), Dialogue);
                    GetWorldTimerManager().SetTimer(DialogueHandle, TimerDelegate, 2.f, false);
                }
            }
        }
    }
}

void ANPCCharacter::EndDialogue(UDialogueComponent* Dialogue)
{
    auto* Player = Cast<AKiraCharacter>(DialogueComponent->Target);

    if (Player && !Player->isSpeakingToANPC)
    {        
        //Stop talking to Kira
        DialogueComponent->Target = nullptr;
        DialogueComponent->isInDialogue = false;

        Dialogue->isInDialogue = false;
        Dialogue->Target = nullptr;
    }
}
