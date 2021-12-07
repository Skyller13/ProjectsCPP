// Fill out your copyright notice in the Description page of Project Settings.


#include "WaypointComponent.h"


#include "Kismet/GameplayStatics.h"
#include "ProjectKapy/ProjectKapyCharacter.h"
#include "ProjectKapy/ProjectKapyGameMode.h"

// Sets default values for this component's properties
UWaypointComponent::UWaypointComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}


// Called when the game starts
void UWaypointComponent::BeginPlay()
{
    Super::BeginPlay();

    // ...
}


// Called every frame
void UWaypointComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
    AutoFadeMarker(RangeFade);
    AutoFadeWaypoint(RangeFade);
}

void UWaypointComponent::CreateWaypoint()
{
    CreateWaypoint_Event(GetOwner());
}

void UWaypointComponent::CreateMarker()
{
    auto cam = Cast<AProjectKapyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->GetCameraComponent();

    CreateMarker_Event(GetOwner(), Cast<USceneComponent>(cam));
}

void UWaypointComponent::AutoFadeMarker(float _range)
{
    if (bCanMarkerFade)
    {
        if (MarkerUI != nullptr)
        {
            auto character = Cast<AProjectKapyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

            if (character)
            {
                float distance = FVector::Distance(character->GetActorLocation(), GetOwner()->GetActorLocation());
                float crossDist = (1.f - (distance / _range));

                if (crossDist > 0.f)
                {
                    float newSize = crossDist * 35.f;
                    MarkerUI->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, crossDist));
                    ModifyImageSize(MarkerUI, newSize, newSize);
                }
            }
        }
    }
}

void UWaypointComponent::AutoFadeWaypoint(float _range)
{
    if (bCanWaypointFade)
    {
        if (WaypointUI != nullptr)
        {
            auto character = Cast<AProjectKapyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

            if (character)
            {
                float distance = FVector::Distance(character->GetActorLocation(), GetOwner()->GetActorLocation());
                float crossDist = (1.f - (distance / _range));

                if (crossDist > 0.f)
                {
                    MarkerUI->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, crossDist));
                }
            }
        }
    }
}

void UWaypointComponent::RemoveWaypoint()
{
    if (WaypointUI != nullptr)
    {
        WaypointUI->RemoveFromParent();
        WaypointUI = nullptr;
    }
}

void UWaypointComponent::RemoveMarker()
{
    RemoveMarker_Event(MarkerUI);
}

UUserWidget* UWaypointComponent::GetMarkerUI()
{
    return MarkerUI;
}

UUserWidget* UWaypointComponent::GetWaypointUI()
{
    return WaypointUI;
}
