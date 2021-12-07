// Fill out your copyright notice in the Description page of Project Settings.


#include "LocationActor.h"


#include "Kismet/GameplayStatics.h"
#include "ProjectKapy/KiraCharacter.h"
#include "ProjectKapy/NPCCharacter.h"
#include "ProjectKapy/ProjectKapyCharacter.h"
#include "ProjectKapy/Gameplay/Character/Component/QuestLogComponent.h"
#include "ProjectKapy/Gameplay/Quest/Quest.h"

// Sets default values
ALocationActor::ALocationActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
    Sphere->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALocationActor::BeginPlay()
{
    Super::BeginPlay();

    //Add Event Overlap 
    if (Sphere)
    {
        Sphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &ALocationActor::OnSphereBeginOverlap);
    }
}

// Called every frame
void ALocationActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ALocationActor::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                          const FHitResult& SweepResult)
{
    auto player = Cast<AKiraCharacter>(OtherActor);

    //first
    if (player)
    {
        auto questLog = player->QuestLog;

        if (questLog)
        {
            auto quests = player->QuestLog->Quests;

            if (quests.Num() > 0)
            {
                for (auto Element : quests)
                {
                    Element->CheckLocationObjective(this);
                }
            }
        }
    }

    //then
    auto npc = Cast<ANPCCharacter>(OtherActor);

    if (npc)
    {
        const auto playerCharacter = Cast<AKiraCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

        if (playerCharacter)
        {
            const auto questLog = playerCharacter->QuestLog;

            if (questLog)
            {
                auto quests = playerCharacter->QuestLog->Quests;

                if (quests.Num() > 0)
                {
                    for (auto Element : quests)
                    {
                        Element->CheckNPCAtLocation(npc, this);
                    }
                }
            }
        }
    }
}
