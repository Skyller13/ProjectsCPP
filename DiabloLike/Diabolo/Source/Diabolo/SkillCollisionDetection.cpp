// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillCollisionDetection.h"

ASkillCollisionDetection::ASkillCollisionDetection()
{
}

void ASkillCollisionDetection::BeginPlay()
{
    Super::BeginPlay();
}

void ASkillCollisionDetection::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void ASkillCollisionDetection::TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::TriggerBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ASkillCollisionDetection::TriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    Super::TriggerEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
