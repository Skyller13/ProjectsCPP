// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerAI.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectKapy/ProjectKapyCharacter.h"
#include "ProjectKapy/Gameplay/Combat/LifeSystemComponent.h"
#include "ProjectKapy/Graphic/DayNightCycleManager.h"

ASpawnerAI::ASpawnerAI()
{
	BoxOfSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxOfSpawn"));
	BoxOfSpawn->SetupAttachment(RootComponent);

	PrimaryActorTick.bCanEverTick = true;
}

ASpawnerAI::~ASpawnerAI()
{
}

void ASpawnerAI::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADayNightCycleManager::StaticClass(), FoundActors);
	if(FoundActors.Num() > 0 )
		DayNightManager = Cast<ADayNightCycleManager>(FoundActors[0]);

	if(SpawnableActors.Max() != 0)
	{
		const int32 TempRand = FMath::RandRange(SpawnBeginPlayMin, SpawnBeginPlayMax);
		for(int32 i = 0; i < TempRand; i++)
		{
			const int32 PercentageSpawn = FMath::RandRange(0,100);
			int32 LastPercentage = 0;
			for (int32 Index = 0; Index < SpawnableActors.Num(); Index++)
			{
				if(DayNightManager && (DayNightManager->GetDayNightMoment() == DAY || DayNightManager->GetDayNightMoment() == MORNING))
					LastPercentage += SpawnableActors[Index].PercentageDay;
				else
					LastPercentage += SpawnableActors[Index].PercentageNight;
				if(PercentageSpawn <= LastPercentage)
				{
					const float TempX = BoxOfSpawn->GetScaledBoxExtent().X;
					const float TempY = BoxOfSpawn->GetScaledBoxExtent().Y;
					const FVector TempVector = FVector(FMath::RandRange(-TempX / 2.0f, TempX / 2.0f), FMath::RandRange(-TempY / 2.0f, TempY / 2.0f), 0.0f);
					AActor* TempActor = GetWorld()->SpawnActor<AActor>(SpawnableActors[Index].ActorClass, BoxOfSpawn->GetComponentLocation() + TempVector, BoxOfSpawn->GetComponentRotation(), FActorSpawnParameters());
					if(TempActor != nullptr)
					{
						TempActor->OnDestroyed.AddDynamic(this, &ASpawnerAI::RemoveActor);
						SpawnedActors.Add(FActorSpawnable(SpawnableActors[Index].ActorClass, TempActor, SpawnableActors[Index].PercentageDay, SpawnableActors[Index].PercentageNight,SpawnableActors[Index].IsIndestructible));
					}
					Index = SpawnableActors.Num();
				}
			}
        }
	}
}

#if WITH_EDITOR
void ASpawnerAI::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	bool NeedChange = true;
	while(NeedChange)
	{
		int32 PercentDayMax = 0;
		int32 PercentNightMax = 0;
		NeedChange = false;
		for(int32 i = 0; i < SpawnableActors.Num(); i++)
		{
			PercentDayMax += SpawnableActors[i].PercentageDay;
			PercentNightMax += SpawnableActors[i].PercentageNight;
			if(PercentDayMax > 100 && SpawnableActors[i].PercentageDay > 0)
			{
				SpawnableActors[i].PercentageDay--;
				NeedChange = true;
			}
			if(PercentNightMax > 100 && SpawnableActors[i].PercentageNight > 0)
			{
				SpawnableActors[i].PercentageNight--;
				NeedChange = true;
			}
		}
	}
}
#endif

void ASpawnerAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult Hit;
	ACharacter* CharacterPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	TimerSpawn += DeltaTime;
	if(TimerSpawn >= TimeBetweenEachSpawn && SpawnableActors.Max() != 0 && SpawnedActors.Num() < ActorsMax
		&& SpawnedActors.Num() + NumberMaxRespawn <= ActorsMax
		&& CharacterPlayer != nullptr
		&& FVector::Distance(BoxOfSpawn->GetComponentLocation(), CharacterPlayer->GetActorLocation()) > DistanceMaxRespawn
		&& GetWorld()->LineTraceSingleByChannel(Hit, CharacterPlayer->GetActorLocation() + CharacterPlayer->GetActorForwardVector() * 100.0f, BoxOfSpawn->GetComponentLocation(), ECC_Camera)
		&& CharacterPlayer->FindComponentByClass<UCameraComponent>() != nullptr
		&& FVector::DotProduct(CharacterPlayer->FindComponentByClass<UCameraComponent>()->GetComponentRotation().Vector().GetSafeNormal(), (BoxOfSpawn->GetComponentLocation() - CharacterPlayer->GetActorLocation()).GetSafeNormal()) <= 0.0f)
	{
		const int32 TempInt = (RandomRespawn ? FMath::RandRange(1, NumberMaxRespawn) : NumberMaxRespawn);
		for(int32 i = 0; i < TempInt; i++)
		{
			const int32 PercentageSpawn = FMath::RandRange(0,100);
			int32 LastPercentage = 0;
			for (int32 Index = 0; Index < SpawnableActors.Num(); Index++)
			{
				if(DayNightManager && (DayNightManager->GetDayNightMoment() == DAY || DayNightManager->GetDayNightMoment() == MORNING))
					LastPercentage += SpawnableActors[Index].PercentageDay;
				else
					LastPercentage += SpawnableActors[Index].PercentageNight;
				if(PercentageSpawn <= LastPercentage)
				{
					const float TempX = BoxOfSpawn->GetScaledBoxExtent().X;
					const float TempY = BoxOfSpawn->GetScaledBoxExtent().Y;
					const FVector TempVector = FVector(FMath::RandRange(-TempX / 2.0f, TempX / 2.0f), FMath::RandRange(-TempY / 2.0f, TempY / 2.0f), 0.0f);
					AActor* TempActor = GetWorld()->SpawnActor<AActor>(SpawnableActors[Index].ActorClass, BoxOfSpawn->GetComponentLocation() + TempVector, BoxOfSpawn->GetComponentRotation(), FActorSpawnParameters());
					if(TempActor != nullptr)
					{
						TempActor->OnDestroyed.AddDynamic(this, &ASpawnerAI::RemoveActor);
						SpawnedActors.Add(FActorSpawnable(SpawnableActors[Index].ActorClass, TempActor, SpawnableActors[Index].PercentageDay, SpawnableActors[Index].PercentageNight, SpawnableActors[Index].IsIndestructible));
					}
					Index = SpawnableActors.Num();
				}
			}
		}
		TimerSpawn = 0.0f;
	}
}

void ASpawnerAI::EmptySpawn(bool EvenIndestructible)
{
	for (int32 Index = SpawnedActors.Num()-1; Index >= 0; --Index)
	{
		if (SpawnedActors[Index].Actor != nullptr && (!SpawnedActors[Index].IsIndestructible || EvenIndestructible))
			SpawnedActors[Index].Actor->Destroy();
	}
}

void ASpawnerAI::RemoveActor(AActor* Actor)
{
	for (int32 Index = SpawnedActors.Num()-1; Index >= 0; --Index)
	{
		AProjectKapyCharacter* SpawnedActor = Cast<AProjectKapyCharacter>(SpawnedActors[Index].Actor);
		if (SpawnedActor != nullptr && SpawnedActor == Actor)
			SpawnedActors.RemoveAt(Index, 1);
	}
}

TArray<AActor*> ASpawnerAI::SpawnQuest(TMap<TSubclassOf<AActor>, int32> ListActors)
{
	EmptySpawn(true);

	TArray<AActor*> TActor;

	for (const TPair<TSubclassOf<AActor>, int32>& Pair : ListActors)
	{
		for(int32 j = 0; j < Pair.Value; j++)
		{
			const float TempX = BoxOfSpawn->GetScaledBoxExtent().X;
			const float TempY = BoxOfSpawn->GetScaledBoxExtent().Y;
			const FVector TempVector = FVector(FMath::RandRange(-TempX / 2.0f, TempX / 2.0f), FMath::RandRange(-TempY / 2.0f, TempY / 2.0f), 0.0f);
			AActor* TempActor = GetWorld()->SpawnActor<AActor>(Pair.Key, BoxOfSpawn->GetComponentLocation() + TempVector, BoxOfSpawn->GetComponentRotation(), FActorSpawnParameters());
			if(TempActor != nullptr)
			{
				TempActor->OnDestroyed.AddDynamic(this, &ASpawnerAI::RemoveActor);
				SpawnedActors.Add(FActorSpawnable(Pair.Key, TempActor, 0, 0, true));
				TActor.Add(TempActor);
			}
		}
	}

	return TActor;
}
