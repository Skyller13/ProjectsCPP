// Fill out your copyright notice in the Description page of Project Settings.


#include "LifeSystemComponent.h"


#include "AntiVirusTacticalGameMode.h"
#include "AntiVirusTacticalPlayerController.h"
#include "VirusCharacter.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
ULifeSystemComponent::ULifeSystemComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
    // ...
}


// Called when the game starts
void ULifeSystemComponent::BeginPlay()
{
    Super::BeginPlay();

    // ...

    if (!HealthComponent)
    {
        auto* Pawn = Cast<AAntiVirusTacticalCharacter>(GetOwner());

        if (Pawn)
        {
            HealthComponent = Pawn->LifeWidgetComponent;
        }
    }
}


// Called every frame
void ULifeSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...

    if (isDead)
    {
        timerDeath += DeltaTime;

        if (timerDeath >= maxTimerDeath)
        {
            GetOwner()->Destroy();
        }
    }
}

void ULifeSystemComponent::AddHealth(int32 _value)
{
    CurrentHealth += _value;

    if (CurrentHealth > HealthMax)
    {
        CurrentHealth = HealthMax;
    }

    auto character = Cast<ACharacter>(GetOwner());

    if (character)
    {
        character->PlayAnimMontage(HealAnimation);
    }

    auto pController = Cast<AAntiVirusTacticalCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void ULifeSystemComponent::RemoveHealth(int32 _value)
{
    if (CurrentHealth <= 0)
        return;

    auto character = Cast<ACharacter>(GetOwner());

    if (character)
    {
        if (HitAnimation)
            character->PlayAnimMontage(HitAnimation);
    }

    CurrentHealth -= _value;

    if (CurrentHealth <= 0)
    {
        //death 
        CurrentHealth = 0;

        if (DeathAnimation)
        {
            character->PlayAnimMontage(DeathAnimation);            
        }

        OnOwnerDeath();
    }

    OnDamageReceive.Broadcast(_value);

    auto pController = Cast<AAntiVirusTacticalCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

bool ULifeSystemComponent::IsOwnerAlive() const
{
    auto* character = Cast<AAntiVirusTacticalCharacter>(GetOwner());
    auto* AICharacter = Cast<AVirusCharacter>(GetOwner());

    if (character && CurrentHealth > 0)
    {
    	return true;
    }
    
    if (AICharacter && CurrentHealth > 0)
    {
    	return true;
    }

    return false;
}

void ULifeSystemComponent::OnOwnerDeath()
{
    if (!isDead)
    {
        auto* gamemode = Cast<AAntiVirusTacticalGameMode>(GetWorld()->GetAuthGameMode());

        if (gamemode)
        {
            auto* player = Cast<AAntiVirusTacticalCharacter>(GetOwner());
            auto* virus = Cast<AVirusCharacter>(GetOwner());

            if (player)
            {
                isDead = true;

                auto* OwnerController = Cast<AAntiVirusTacticalPlayerController>(player->GetController());

                if (OwnerController)
                {
                    OwnerController->RemovePawnFromList(player);
                    OwnerController->OnSwitchCharacter();
                }

                player->playerNode->LinkedActor = nullptr;
                player->playerNode = nullptr;
            }

            if (virus)
            {
                isDead = true;
                
                for (int i = 0; i < gamemode->Enemies.Num(); i++)
                {
                    if (gamemode->Enemies[i] == virus)
                    {
                        GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Dead dog !"));
                        gamemode->Enemies.RemoveAt(i);
                        virus->LinkedNode->LinkedActor = nullptr;
                        virus->LinkedNode = nullptr;
                    }
                }
            }
        }
    }
}
