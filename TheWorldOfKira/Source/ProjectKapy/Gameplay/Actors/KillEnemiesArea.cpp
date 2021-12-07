// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEnemiesArea.h"


#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectKapy/KiraCharacter.h"
#include "ProjectKapy/Gameplay/Character/Component/QuestLogComponent.h"
#include "ProjectKapy/Gameplay/Combat/LifeSystemComponent.h"
#include "ProjectKapy/Gameplay/Quest/Quest.h"

// Sets default values
AKillEnemiesArea::AKillEnemiesArea()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    USceneComponent* scene = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
    scene->SetupAttachment(RootComponent);

    Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    Box->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AKillEnemiesArea::BeginPlay()
{
    Super::BeginPlay();
}

void AKillEnemiesArea::SpawnEnemiesAtLocation()
{
    if (EnemiesToSpawn.Num() > 0)
    {
        for (auto Element : EnemiesToSpawn)
        {           
            for (int i = 0; i < Element.Value; i++)
            {
                FTransform* transform = new FTransform();

                FVector Position = GetActorLocation();
                const FVector BoxScale = Box->GetScaledBoxExtent();
                FRotator Rotator = FRotator();

                Position.X += FMath::RandRange(-BoxScale.X * 0.5f, BoxScale.X * 0.5f);
                Position.Y += FMath::RandRange(-BoxScale.Y * 0.5f, BoxScale.Y * 0.5f);

                const auto character = GetWorld()->SpawnActor(Element.Key, &Position, &Rotator);

                auto Enemy = Cast<AProjectKapyCharacter>(character);

                if (Enemy != nullptr)
                {                    
                    if (!EnemiesArray.Contains(Enemy))
                    {
                        if (Enemy->LifeSystem != nullptr)
                        {
                            EnemiesArray.AddUnique(Enemy);

                            Enemy->LifeSystem->OnDeath.AddUniqueDynamic(this, &AKillEnemiesArea::CheckQuestArea);
                        }
                    }
                }
            }
        }
    }
}

// Called every frame
void AKillEnemiesArea::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AKillEnemiesArea::CheckQuestArea()
{
    //This function remove the enemy killed and call the function quest validation
    const auto player = Cast<AKiraCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    for (int i = 0; i < EnemiesArray.Num(); i++)
    {
        if (EnemiesArray[i] == nullptr
            || EnemiesArray[i]->IsPendingKill()
            || EnemiesArray[i]->GetLifeSystem()->GetHealthPercent() == 0.f)
        {
            EnemiesArray.RemoveAt(i);
        }
    }

    if (player
        && EnemiesArray.Num() <= 0)
    {
        auto quests = player->QuestLog->Quests;

        for (auto Element : quests)
        {
            Element->CheckEnemiesInArea(this);
        }
    }
}

void AKillEnemiesArea::SetQuestReference(AQuest* _ref)
{
    if (_ref != nullptr)
    {
        QuestRef = _ref;
    }
}

void AKillEnemiesArea::LinkSpawnTriggerEvent()
{
    if (QuestRef != nullptr)
        QuestRef->OnSpawnTrigger.AddDynamic(this, &AKillEnemiesArea::SpawnEnemiesAtLocation);
}
