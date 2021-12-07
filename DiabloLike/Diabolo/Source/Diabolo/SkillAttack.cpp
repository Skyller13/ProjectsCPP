// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillAttack.h"
#include "SkillCollisionDetection.h"
#include "SkillManagerComponent.h"
#include "DiaboloCharacter.h"

void USkillAttack::Execute()
{
    Super::Execute();
}

void USkillAttack::SkillBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::SkillBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
    //damages
}

void USkillAttack::SpawnDetectionMesh()
{
    if (SkillManagerComponent)
    {
        ASkillCollisionDetection* detection = nullptr;

        detection = SkillManagerComponent->GetWorld()->SpawnActorDeferred<ASkillCollisionDetection>(detectionMesh, SkillManagerComponent->GetOwner()->GetTransform());

        if (detection)
        {
            detection->Init(this);
            detection->FinishSpawning(SkillManagerComponent->GetOwnerCharacter()->GetTransform());
        }
    }
}

void USkillAttack::SpawnDetectionMeshSpecificTransform(FTransform _transform)
{
    if (SkillManagerComponent)
    {
        ASkillCollisionDetection* detection = nullptr;

        detection = SkillManagerComponent->GetWorld()->SpawnActorDeferred<ASkillCollisionDetection>(detectionMesh, _transform);

        if (detection)
        {
            detection->Init(this);
            detection->FinishSpawning(_transform);
        }
    }
}
