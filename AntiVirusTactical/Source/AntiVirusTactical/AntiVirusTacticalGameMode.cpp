// Copyright Epic Games, Inc. All Rights Reserved.

#include "AntiVirusTacticalGameMode.h"
#include "AntiVirusTacticalPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

void AAntiVirusTacticalGameMode::BeginPlay()
{
    Super::BeginPlay();

    //Create Main Widget
    if (MainWidget != nullptr)
    {
        MainWidgetUI = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), MainWidget);
        MainWidgetUI->AddToViewport();
    }

    //Get grid map
    grid = UGameplayStatics::GetActorOfClass(GetWorld(), GridActor);

    Controller = Cast<AAntiVirusTacticalPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void AAntiVirusTacticalGameMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (Enemies.Num() == 0)
    {
        //Virus is out, you win
        OnWinEvent();
    }

    if (Controller && Controller->GetPawns().Num() <= 0)
    {
        OnLoseEvent();
    }
}

AAntiVirusTacticalGameMode::AAntiVirusTacticalGameMode()
{
    // use our custom PlayerController class
    PlayerControllerClass = AAntiVirusTacticalPlayerController::StaticClass();

    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(
        TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
    if (PlayerPawnBPClass.Class != NULL)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
}

void AAntiVirusTacticalGameMode::ChangeTurn()
{
    isPlayerTurn = !isPlayerTurn;

    if (isPlayerTurn)
    {
        TArray<AActor*> Allied;

        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAntiVirusTacticalCharacter::StaticClass(), Allied);

        if (Allied.Num() > 0)
        {
            for (auto& Element : Allied)
            {
                auto* character = Cast<AAntiVirusTacticalCharacter>(Element);

                if (character && character->lifeSystemComponent)
                {
                    if (character->lifeSystemComponent->IsOwnerAlive())
                        character->hasPlayed = false;
                }
            }
        }
        else
        {
            OnLoseEvent();
        }
    }
    else
    {
        FTimerHandle tempHandle;
        GetWorld()->GetTimerManager().SetTimer(tempHandle, this,
                                               &AAntiVirusTacticalGameMode::WaitUntilEnemyStartPlaying, 1.f, false);
    }
}

void AAntiVirusTacticalGameMode::OnEnemiesEndTurn()
{
    bool isEnemyTurnEnded = true;

    for (int i = 0; i < Enemies.Num(); i++)
    {
        if (!Enemies[i]->hasPlayed)
        {
            isEnemyTurnEnded = false;
        }
    }

    if (isEnemyTurnEnded)
    {
        ChangeTurn();
    }
}

void AAntiVirusTacticalGameMode::WaitUntilEnemyStartPlaying()
{
    if (Enemies.Num() > 0)
    {
        for (int i = 0; i < Enemies.Num(); i++)
        {
            if (Enemies[i] != nullptr)
                Enemies[i]->hasPlayed = false;
        }
    }
}
