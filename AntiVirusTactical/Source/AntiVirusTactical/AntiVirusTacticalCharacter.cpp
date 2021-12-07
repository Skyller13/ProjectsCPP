// Copyright Epic Games, Inc. All Rights Reserved.

#include "AntiVirusTacticalCharacter.h"

#include "AntiVirusTacticalGameMode.h"
#include "AntiVirusTacticalPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "AntiVirusTactical/GridNode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Obstacle_Interface.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AAntiVirusTacticalCharacter::AAntiVirusTacticalCharacter()
{
    // Set size for player capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // Don't rotate character to camera direction
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
    GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
    GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->bSnapToPlaneAtStart = true;

    // Create a camera boom...
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
    CameraBoom->TargetArmLength = 800.f;
    CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
    CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

    // Create a camera...
    TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
    TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

    //Create widget component
    LifeWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Life Widget Component"));
    LifeWidgetComponent->SetupAttachment(RootComponent);

    //Create life system component
    lifeSystemComponent = CreateDefaultSubobject<ULifeSystemComponent>(TEXT("Life System Component"));

    // Activate ticking in order to update the cursor every frame.
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;
}

void AAntiVirusTacticalCharacter::BeginPlay()
{
    Super::BeginPlay();

    OnBeginTurnEvent.AddUniqueDynamic(this, &AAntiVirusTacticalCharacter::DisplayCharacterMovementRange);
    OnSwitchEvent.AddUniqueDynamic(this, &AAntiVirusTacticalCharacter::SwitchBetweenCharacters);
}

void AAntiVirusTacticalCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        auto* controlledPawn = PC->GetPawn();

        if (!controlledPawn)
            return;

        auto* currentPlayer = Cast<AAntiVirusTacticalCharacter>(controlledPawn);

        //Check the current possessed pawn to play with
        if (currentPlayer && !currentPlayer->hasPlayed)
        {
            auto mode = Cast<AAntiVirusTacticalGameMode>(GetWorld()->GetAuthGameMode());

            auto* grid = mode->GetGrid();

            FHitResult TraceHitResult;

            //If the cursor isn't on a tile, then hide the cursor
            const bool isHitting = PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);

            if (TraceHitResult.Actor != nullptr)
            {
                if (TraceHitResult.Actor->Implements<UObstacle_Interface>())
                {
                    HideTile_Event(grid);
                    return;
                }
            }

            //Display Node under cursor 
            if (isHitting
                && Cast<AAntiVirusTacticalPlayerController>(Controller)->GetIsPlayerTurn())
            {
                const FVector HitLocation = TraceHitResult.Location;

                SetLocationToTile_Event(grid, HitLocation);
            }
            else
            {
                //Hide when the cursor hit an obstacle
                HideTile_Event(grid);
            }
        }
    }
}

void AAntiVirusTacticalCharacter::DisplayCharacterMovementRange()
{
    auto* pawn = Controller->GetPawn();

    if (pawn != nullptr)
    {
        auto* player = Cast<AAntiVirusTacticalCharacter>(pawn);

        if (player->playerNode != nullptr)
        {
            CheckNodeInRange_Event(playerNode);
        }
    }
}

bool AAntiVirusTacticalCharacter::Attack()
{
    auto* control = Cast<AAntiVirusTacticalPlayerController>(GetController());

    if (!control->currentSelection)
        return false;

    auto* enemy = Cast<AVirusCharacter>(control->currentSelection->LinkedActor);

    if (control->currentSelection && enemy)
    {
        //Rotate to face the current enemy
        const FVector location = control->currentSelection->GetActorLocation();

        const auto rotator = UKismetMathLibrary::FindLookAtRotation(
            GetActorLocation(), location);

        SetActorRotation(FRotator(0.f, rotator.Yaw, 0.f));

        if (AttackAnimation)
            PlayAnimMontage(AttackAnimation);

        enemy->lifeSystemComponent->RemoveHealth(damage);

        control->currentSelection->OnNodeUnselected;
        control->currentSelection->OnNodeReset;
        control->currentSelection = nullptr;
        return true;
    }

    return false;
}