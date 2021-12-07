// Fill out your copyright notice in the Description page of Project Settings.


#include "HitboxAbilityActor.h"

#include "ProjectKapy/Gameplay/Abilities/AbilityBase.h"

AHitboxAbilityActor::AHitboxAbilityActor()
{
	USceneComponent* Root = CreateDefaultSubobject<UStaticMeshComponent>("DefaultRoot");
	SetRootComponent(Root);
	Root->bEditableWhenInherited = true;
	HitboxMesh = CreateDefaultSubobject<UStaticMeshComponent>("HitboxMesh");
	HitboxMesh->bEditableWhenInherited = true;
	HitboxMesh->SetupAttachment(GetRootComponent());
}

void AHitboxAbilityActor::BeginPlay()
{
	Super::BeginPlay();
	if (HitboxMesh)
	{
		HitboxMesh->OnComponentBeginOverlap.AddUniqueDynamic(this, &AHitboxAbilityActor::OnAbilityActorBeginOverlap);
		HitboxMesh->OnComponentEndOverlap.AddUniqueDynamic(this, &AHitboxAbilityActor::OnAbilityActorEndOverlap);
	}
}

void AHitboxAbilityActor::OnAttackInterrupted()
{
	BP_OnAttackInterrupted();
}

void AHitboxAbilityActor::OnAbilityActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                     bool bFromSweep, const FHitResult& SweepResult)
{
	if (ActorsHit.Contains(OtherActor))
	{
		return;
	}

	ActorsHit.Add(OtherActor);
	Super::OnAbilityActorBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep,
	                                  SweepResult);
}
