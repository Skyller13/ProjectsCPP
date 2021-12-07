// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillLinearProjectile.h"

#include "DiaboloCharacter.h"
#include "SkillBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ASkillLinearProjectile::ASkillLinearProjectile()
{
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Component");

    if (ProjectileMovementComponent)
    {
        if (RootComponent)
        {
            ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
            ProjectileMovementComponent->bInitialVelocityInLocalSpace = false;
            ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
        }
    }
}

void ASkillLinearProjectile::BeginPlay()
{
    Super::BeginPlay();
}

void ASkillLinearProjectile::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    distanceTravelled += FVector::Distance(lastFramePosition, GetActorLocation());
    lastFramePosition = GetActorLocation();

    if (distanceTravelled >= maxRange)
    {
        Destroy();
    }
}

void ASkillLinearProjectile::Init(USkillBase* _skill, FVector _direction, float _speed, float _maxRange)
{
    Super::Init(_skill);
    Skill = _skill;
    distanceTravelled = 0.0f;
    maxRange = _maxRange;
    lastFramePosition = GetActorLocation();
    
    if (Skill)
    {
        if (!_direction.IsNearlyZero())
        {
            _direction.Normalize();
            ProjectileMovementComponent->Velocity = _direction * _speed;
        }
    }
}

void ASkillLinearProjectile::TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                 const FHitResult& SweepResult)
{
    Super::TriggerBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ASkillLinearProjectile::TriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    Super::TriggerEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
