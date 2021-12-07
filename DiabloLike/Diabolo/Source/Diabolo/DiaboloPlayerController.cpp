// Copyright Epic Games, Inc. All Rights Reserved.

#include "DiaboloPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "DiaboloCharacter.h"
#include "Blueprint/UserWidget.h"
#include "LevellingSystem.h"
#include "HealthComponent.h"
#include "Kismet/KismetMathLibrary.h"

ADiaboloPlayerController::ADiaboloPlayerController()
{
    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ADiaboloPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (!MainUI)
    {
        MainUI = CreateWidget<UUserWidget>(this, MainWidget, FName("mainWidget"));
        ExperienceUI = CreateWidget<UUserWidget>(MainUI, ExpWidget, FName("expWidget"));
        HealthUI = CreateWidget<UUserWidget>(MainUI, HealthWidget, FName("HealthWidget"));

        if (MainUI)
        {
            //Add widgets to viewport
            MainUI->AddToViewport();
            ExperienceUI->AddToViewport();
            HealthUI->AddToViewport();

            //Assign new UI in main UI and set anchors/positions
            ExperienceUI->SetPositionInViewport(FVector2D(30, 30));
            HealthUI->SetPositionInViewport(FVector2D(64, 996));
            HealthUI->SetDesiredSizeInViewport(FVector2D(300, 30));
        }
    }
}

void ADiaboloPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);
}

void ADiaboloPlayerController::MoveToMouseCursor()
{
    if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
    {
        if (ADiaboloCharacter* MyPawn = Cast<ADiaboloCharacter>(GetPawn()))
        {
            if (MyPawn->GetCursorToWorld())
            {
                UAIBlueprintHelperLibrary::SimpleMoveToLocation(
                    this, MyPawn->GetCursorToWorld()->GetComponentLocation());
            }
        }
    }
    else
    {
        // Trace to see what is under the mouse cursor
        FHitResult Hit;
        GetHitResultUnderCursor(ECC_Visibility, false, Hit);

        if (Hit.bBlockingHit)
        {
            // We hit something, move there
            SetNewMoveDestination(Hit.ImpactPoint);
        }
    }
}

void ADiaboloPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
    APawn* const MyPawn = GetPawn();
    if (MyPawn)
    {
        float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

        // We need to issue move command only if far enough in order for walk animation to play correctly
        if ((Distance > 120.0f))
        {
            UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
        }
    }
}

void ADiaboloPlayerController::OnSetDestinationPressed()
{
    // set flag to keep updating destination until released
    bMoveToMouseCursor = true;
}

void ADiaboloPlayerController::OnSetDestinationReleased()
{
    // clear flag to indicate we should stop updating the destination
    bMoveToMouseCursor = false;
}

void ADiaboloPlayerController::OnSkill0Requested()
{
    auto character = Cast<ADiaboloCharacter>(GetCharacter());

    if (character)
    {
        //to change !
        character->levelSystem->AddExperience(200.0f);
    }
}

void ADiaboloPlayerController::OnSkill1Requested()
{
    auto character = Cast<ADiaboloCharacter>(GetCharacter());

    if (character)
    {
        //to change !
        character->HealthComponent->AddHealth(20.0f);
    }
}

void ADiaboloPlayerController::OnSkill2Requested()
{
    auto character = Cast<ADiaboloCharacter>(GetCharacter());

    if (character)
    {
        //to change !
        character->HealthComponent->AddHealth(-20.0f);
    }
}
