// Copyright Epic Games, Inc. All Rights Reserved.

#include "AntiVirusTacticalPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AntiVirusTactical/GridNode.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "AntiVirusTacticalCharacter.h"
#include "AntiVirusTacticalGameMode.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

AAntiVirusTacticalPlayerController::AAntiVirusTacticalPlayerController()
{
    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AAntiVirusTacticalPlayerController::BeginPlay()
{
    Super::BeginPlay();

    //Add every pawn to the list to navigate through
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAntiVirusTacticalCharacter::StaticClass(), PawnList);

    if (PawnList.Num() > 0)
    {
        Possess(Cast<AAntiVirusTacticalCharacter>(PawnList[0]));

        if (PawnList[0])
        {
            auto* player = Cast<AAntiVirusTacticalCharacter>(PawnList[0]);

            if (player)
            {
                player->DisplayCharacterMovementRange();
            }
        }
    }

    CurrentAction = ECurrentAction::Walk;
}

void AAntiVirusTacticalPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);
}

void AAntiVirusTacticalPlayerController::SetupInputComponent()
{
    // set up gameplay key bindings
    Super::SetupInputComponent();

    InputComponent->BindAction("SetDestination", IE_Pressed, this,
                               &AAntiVirusTacticalPlayerController::OnSetDestinationPressed);

    InputComponent->BindAction("SwitchCharacter", IE_Released, this,
                               &AAntiVirusTacticalPlayerController::OnSwitchCharacter);
}

void AAntiVirusTacticalPlayerController::SelectTileUnderMouseCursor()
{
    const auto player = Cast<AAntiVirusTacticalCharacter>(GetPawn());

    // Trace to see what is under the mouse cursor
    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, false, Hit);

    if (Hit.bBlockingHit)
    {
        if (player && !player->hasPlayed)
        {
            if (CurrentAction == Walk)
            {
                const float pathLength = GetPathToNode_Event(player->playerNode, Hit.ImpactPoint);

                //Check distance between nodes to check if player has the range to move
                if (pathLength > 0 && pathLength <= (player->rangeToMoveOn * 100.f))
                {
                    //Keep the node selected in memory and display it
                    ChangeNodeSelection_Event(Hit.ImpactPoint);
                }
            }
            else if (CurrentAction == Attack)
            {
                if (player->playerNode)
                    ChangeNodeSelection_Event(Hit.ImpactPoint);
            }
        }
    }
}

void AAntiVirusTacticalPlayerController::SetPlayerToMoveOnSelectedNode()
{
    APawn* const MyPawn = GetPawn();

    if (MyPawn)
    {
        const auto player = Cast<AAntiVirusTacticalCharacter>(MyPawn);

        if (player && !player->hasPlayed
            && currentSelection)
        {
            //Get capsule radius
            float radiusX = player->GetCapsuleComponent()->GetScaledCapsuleRadius();
            float radiusY = radiusX;

            //Get Node location
            AActor* NodeActor = Cast<AActor>(currentSelection);
            const FVector currentNodeLocation = NodeActor->GetActorLocation();

            if (currentNodeLocation.X < player->GetActorLocation().X)
            {
                radiusX = - radiusX;
            }

            if (currentNodeLocation.Y < player->GetActorLocation().Y)
            {
                radiusY = - radiusY;
            }

            const FVector finalPos = FVector(currentNodeLocation.X + radiusX, currentNodeLocation.Y + radiusY,
                                             currentNodeLocation.Z);

            if (finalPos.X != 0.f && finalPos.Y != 0.f)
            {
                UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, finalPos);

                currentSelection->OnNodeUnselected.Broadcast();

                player->playerNode->LinkedActor = nullptr;
                currentSelection->LinkedActor = player;

                player->playerNode = currentSelection;
                currentSelection = nullptr;
            }
        }
    }
}

void AAntiVirusTacticalPlayerController::OnSetDestinationPressed()
{
    //Check if it's player or AI turn
    auto* pawn = GetPawn();

    if (pawn)
    {
        SelectTileUnderMouseCursor();
    }
}

void AAntiVirusTacticalPlayerController::OnSwitchCharacter()
{
    auto* oldPlayer = Cast<AAntiVirusTacticalCharacter>(GetPawn());

    bool isTurnEnded = true;

    /* Check if every character has played */
    for (auto* Element : PawnList)
    {
        auto* character = Cast<AAntiVirusTacticalCharacter>(Element);

        if (character && character->lifeSystemComponent && character->lifeSystemComponent->IsOwnerAlive())
        {
            if (!character->hasPlayed)
            {
                isTurnEnded = false;
                break;
            }
        }
    }

    if (isTurnEnded)
    {
        auto* gamemode = GetWorld()->GetAuthGameMode();

        if (gamemode)
        {
            Cast<AAntiVirusTacticalGameMode>(gamemode)->ChangeTurn();
        }

        return;
    }

    AAntiVirusTacticalCharacter* character = nullptr;

    if (PawnList.IsValidIndex(currentIndexPawn))
    {
        character = Cast<AAntiVirusTacticalCharacter>(PawnList[currentIndexPawn]);
    }
    else
    {
        character = Cast<AAntiVirusTacticalCharacter>(PawnList[0]);
    }

    do
    {
        currentIndexPawn++;

        if (currentIndexPawn >= PawnList.Num())
            currentIndexPawn = 0;

        character = Cast<AAntiVirusTacticalCharacter>(PawnList[currentIndexPawn]);

        if (character == GetPawn() || !character->lifeSystemComponent->isDead)
            break;
    }
    while (character->hasPlayed);

    auto* newPlayer = Cast<AAntiVirusTacticalCharacter>(PawnList[currentIndexPawn]);

    currentSelection->OnNodeReset;
    currentSelection->OnNodeUnselected;
    currentSelection = nullptr;

    if (oldPlayer)
    {
        if (oldPlayer->hasPlayed)
        {
            oldPlayer->OnEndTurnEvent.Broadcast();
        }
        else
        {
            oldPlayer->OnSwitchEvent.Broadcast();
        }
    }

    if (newPlayer && !newPlayer->hasPlayed)
    {
        OnPossessSwitchCamera_Event(newPlayer);
    }
}

bool AAntiVirusTacticalPlayerController::CheckNbCharacterRemains() const
{
    if (PawnList.Num() <= 0)
    {
        return true;
    }

    return false;
}

void AAntiVirusTacticalPlayerController::RemovePawnFromList(AActor* _Pawn)
{
    for (int i = 0; i < PawnList.Num(); i++)
    {
        if (PawnList[i] == _Pawn)
        {
            PawnList.RemoveAt(i);
        }
    }
}
