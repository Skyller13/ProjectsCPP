// Copyright Epic Games, Inc. All Rights Reserved.

#include "NetworkTestProjectile.h"

#include "NetworkTestCharacter.h"
#include "NetworkTestGameState.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

ANetworkTestProjectile::ANetworkTestProjectile()
{
    // Use a sphere as a simple collision representation
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComp->InitSphereRadius(5.0f);
    CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
    CollisionComp->OnComponentHit.AddDynamic(this, &ANetworkTestProjectile::OnHit);
    // set up a notification for when this component hits something blocking

    // Players can't walk on it
    CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
    CollisionComp->CanCharacterStepUpOn = ECB_No;

    // Set as root component
    RootComponent = CollisionComp;

    // Use a ProjectileMovementComponent to govern this projectile's movement
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    ProjectileMovement->UpdatedComponent = CollisionComp;
    ProjectileMovement->InitialSpeed = 3000.f;
    ProjectileMovement->MaxSpeed = 3000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = true;

    // Die after 3 seconds by default
    InitialLifeSpan = 3.0f;
}

void ANetworkTestProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                   FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor != NULL)
    {
        auto character = Cast<ANetworkTestCharacter>(OtherActor);

        if (character)
        {
            auto controller = Cast<ANetPlayerController>(character->GetController());

            if (controller)
            {
                auto gamestate = Cast<ANetworkTestGameState>(GetWorld()->GetGameState());
                
                if (controller->TeamName != team
                    && gamestate != nullptr)
                {
                    if (team == TeamA)
                    {
                        gamestate->TeamScoreA++;
                    }
                    else
                    {
                        gamestate->TeamScoreB++;
                    }

                    Destroy();
                }
            }
        }
    }

    
    // Only add impulse and destroy projectile if we hit a physics
    if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
    {
        OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

        Destroy();
    }
}
