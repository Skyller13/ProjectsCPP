// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityActorBase.h"
#include "ProjectKapy/Gameplay/Abilities/AbilityBase.h"

// Sets default values
AAbilityActorBase::AAbilityActorBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAbilityActorBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAbilityActorBase::Init(UAbilityBase* Ability)
{
	AbilityOwner = Ability;
}

// Called every frame
void AAbilityActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ElapsedTimeSinceSpawn += DeltaTime;
}

void AAbilityActorBase::OnAbilityActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                   bool bFromSweep, const FHitResult& SweepResult)
{
	BP_OnAbilityActorBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (AbilityOwner)
	{
		AbilityOwner->OnAbilityBeginOverlap(this, OverlappedComponent, OtherActor,
		                                    OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
}

void AAbilityActorBase::OnAbilityActorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	BP_OnAbilityActorEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if (AbilityOwner)
	{
		AbilityOwner->OnAbilityEndOverlap(this, OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	}
}

void AAbilityActorBase::OnAbilityActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	BP_OnAbilityActorHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
	if (AbilityOwner)
	{
		AbilityOwner->OnAbilityHit(this, HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
	}
}
