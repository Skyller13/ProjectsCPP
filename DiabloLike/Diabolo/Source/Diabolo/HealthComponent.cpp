// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"


#include "AIControllerBase.h"
#include "DiaboloCharacter.h"
#include "AICharacter.h"
#include "DiaboloPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    // ...
    // 
    //Create widget
    if (!HealthUI)
    {
        auto characterController = Cast<ADiaboloPlayerController>(GetOwner());

        if (characterController)
        {
            HealthUI = characterController->HealthUI;
        }
    }

    tempBuffer = BufferHealth;

    CalculatePercentage();
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...

    //Check max and min values
    if (tempBuffer > HealthMax)
    {
        tempBuffer = HealthMax;
    }
    else if (tempBuffer < 0)
    {
        tempBuffer = 0;
    }

    //Interpolate values thanks to tempBuffer to modify life over time
    if (tempBuffer > CurrentHealth)
    {
        BufferHealth = tempBuffer;
        CurrentHealth = FMath::FInterpTo(CurrentHealth, BufferHealth, DeltaTime, 5.0f);
    }
    else if (tempBuffer <= CurrentHealth)
    {
        CurrentHealth = tempBuffer;
        BufferHealth = FMath::FInterpTo(BufferHealth, CurrentHealth, DeltaTime, 5.0f);
    }

    if (tempBuffer <= 0)
    {
        //Player is now dead
        auto* character = Cast<ADiaboloCharacter>(GetOwner());
        auto* AICharacter = Cast<AAICharacter>(GetOwner());

        if (character
            && character->currentPlayerState == Alive)
        {
            character->currentPlayerState = Dead;
            character->OnPlayerDeath.Broadcast();

            if (character->bIsCrouched)
            {
                character->GetMesh()->PlayAnimation(CrouchDeathAnimation, false);
            }
            else
            {
                int rand = FMath::RandRange(0, StandDeathAnimationList.Max() - 1);

                character->GetMesh()->PlayAnimation(StandDeathAnimationList[rand], false);
            }
        }
        else if (AICharacter && AICharacter->state == Alive)
        {
            AICharacter->state = Dead;
            AICharacter->OnDeathCharacter.Broadcast();

            int rand = FMath::RandRange(0, StandDeathAnimationList.Max() - 1);
            AICharacter->GetMesh()->PlayAnimation(StandDeathAnimationList[rand], false);

            AICharacter->LifeWidgetComponent->DestroyComponent();
            AICharacter->OnDeathCharacter.Broadcast();
            AICharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }
    }

    CalculatePercentage();
}

void UHealthComponent::CalculatePercentage()
{
    CurrentPercentage = CurrentHealth / HealthMax;
}

void UHealthComponent::AddHealth(float _value)
{
    tempBuffer += _value;

    auto character = Cast<ACharacter>(GetOwner());
    
    if (character)
    {
        character->PlayAnimMontage(HealAnimation);
    }

    auto pController = Cast<ADiaboloPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

    if (pController)
    {
        pController->CreateFloatingDamage(_value, FColor::Green, character->GetActorLocation());
    }
}

void UHealthComponent::RemoveHealth(float _value)
{
    if (CurrentHealth <= 0)
        return;
    
    tempBuffer -= _value;

    auto character = Cast<ACharacter>(GetOwner());
    
    if (character)
    {
        character->PlayAnimMontage(HitAnimation);
    }

    OnDamageReceive.Broadcast();

    auto pController = Cast<ADiaboloPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

    if (pController)
    {
        pController->CreateFloatingDamage(_value, FColor::Red, character->GetActorLocation());
    }
}

bool UHealthComponent::IsOwnerAlive() const
{
    auto* character = Cast<ADiaboloCharacter>(GetOwner());
    auto* AICharacter = Cast<AAICharacter>(GetOwner());

    if (character && character->currentPlayerState == Alive)
    {
        return true;
    }

    if (AICharacter && AICharacter->state == Alive)
    {
        return true;
    }

    return false;
}
