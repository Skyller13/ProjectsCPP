// Fill out your copyright notice in the Description page of Project Settings.


#include "PushableBlock.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectKapy/CatCharacterMovement.h"
#include "ProjectKapy/KiraCharacter.h"
#include "ProjectKapy/ProjectKapyCharacter.h"

// Sets default values
APushableBlock::APushableBlock()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SuperMeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);

    //Components
    TriggerBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collide Box"));
    TriggerBoxComponent->SetupAttachment(MeshComponent);
    //
}

// Called when the game starts or when spawned
void APushableBlock::BeginPlay()
{
    Super::BeginPlay();

    //Components
    TriggerBoxComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &APushableBlock::Interact);
    TriggerBoxComponent->OnComponentEndOverlap.AddUniqueDynamic(this, &APushableBlock::StopInteraction);
}

void APushableBlock::Interact(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                              const FHitResult& SweepResult)
{
    auto* player = Cast<AKiraCharacter>(OtherActor);

    if (player && IsValid(player))
    {
        player->linkedBlock = this;
        player->SetCharacterState(STATE_PushingBlock);

        auto move = Cast<UCatCharacterMovement>(player->GetCharacterMovement());

        if (move && IsValid(move))
        {
            MeshComponent->SetMassScale(NAME_None, 125.f);

            auto* movement = Cast<UCatCharacterMovement>(player->GetCharacterMovement());

            if (movement)
            {
                movement->bIsPushingBlock = true;
                movement->isAbleToClimb = false;        
                movement->MaxWalkSpeed = player->PushBlockSpeed;
            }        
        }
    }
}

void APushableBlock::StopInteraction(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    auto* player = Cast<AKiraCharacter>(OtherActor);

    if (player && IsValid(player))
    {
        player->linkedBlock = nullptr;
        player->SetCharacterState(STATE_None);

        auto move = Cast<UCatCharacterMovement>(player->GetCharacterMovement());

        if (move && IsValid(move))
        {
            MeshComponent->SetMassScale(NAME_None, 1000.f);

            auto* movement = Cast<UCatCharacterMovement>(player->GetCharacterMovement());

            if (movement)
            {
                movement->isAbleToClimb = false;
                movement->bIsPushingBlock = false;
                movement->MaxWalkSpeed = player->RunSpeed;
            }
        }
    }
}

// Called every frame
void APushableBlock::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}