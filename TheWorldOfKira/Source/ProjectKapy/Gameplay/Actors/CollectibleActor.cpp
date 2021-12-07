// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectibleActor.h"

#include "Kismet/GameplayStatics.h"
#include "ProjectKapy/KiraCharacter.h"
#include "ProjectKapy/ProjectKapyCharacter.h"

// Sets default values
ACollectibleActor::ACollectibleActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACollectibleActor::BeginPlay()
{
    Super::BeginPlay();

    UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));

    if (mesh)
        mesh->SetGenerateOverlapEvents(false);
}

// Called every frame
void ACollectibleActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACollectibleActor::OnCollect()
{
    auto player = Cast<AProjectKapyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    if (player)
    {
        auto character = Cast<AKiraCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

        if (character != nullptr)
        {
            character->AddItemToCollection(data.ID, 1);

            character->CheckCollectionObjective();

            Destroy();
        }
    }
}

void ACollectibleActor::ActivateCollectible()
{
    UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));

    if (mesh)
    {
        mesh->SetGenerateOverlapEvents(true);
    }
}

void ACollectibleActor::DeactivateCollectible()
{
    UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));

    if (mesh)
    {
        mesh->SetGenerateOverlapEvents(false);
    }
}
