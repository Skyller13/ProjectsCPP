// Fill out your copyright notice in the Description page of Project Settings.


#include "Trampoline.h"

#include "ProjectKapy/KiraCharacter.h"

// Sets default values
ATrampoline::ATrampoline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	MeshComponent->SetupAttachment(RootComponent);
	
	TrampolineTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	TrampolineTrigger->SetupAttachment(MeshComponent);
}

// Called when the game starts or when spawned
void ATrampoline::BeginPlay()
{
	Super::BeginPlay();
	
	TrampolineTrigger->OnComponentBeginOverlap.AddUniqueDynamic(this, &ATrampoline::Bounce);
}

// Called every frame
void ATrampoline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATrampoline::Bounce(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	auto Player = Cast<AKiraCharacter>(OtherActor);
	
	if (Player)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Orange, FString::Printf(TEXT("Velocity : %s"), *Player->GetVelocity().ToString()));

		const FVector velocity = Player->GetVelocity();

		if (velocity.Z < 0)
		{
			Player->LaunchCharacter(BouncingVelocity, false, true);			
		}
	}
}
