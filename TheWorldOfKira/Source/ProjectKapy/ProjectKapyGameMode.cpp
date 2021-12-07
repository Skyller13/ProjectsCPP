// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectKapyGameMode.h"
#include "ProjectKapyCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "GameFramework/PlayerInput.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AProjectKapyGameMode::AProjectKapyGameMode()
{
    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(
        TEXT("/Game/Blueprint/Characters/ThirdPersonCharacter"));
    if (PlayerPawnBPClass.Class != NULL)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
}

void AProjectKapyGameMode::BeginPlay()
{
    Super::BeginPlay();

    MainWidgetUI = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), MainWidget);

    MainWidgetUI->AddToViewport();

    CanvasPanel = Cast<UCanvasPanel>(MainWidgetUI->GetRootWidget());
}

UUserWidget* AProjectKapyGameMode::CreateWaypoint(AActor* _actor) const
{
    if (MainWidgetUI == nullptr || _actor == nullptr)
        return nullptr;

    UUserWidget* tempWidget = CreateWidget<UUserWidget>(MainWidgetUI, WaypointWidget);

    CanvasPanel->AddChild(tempWidget);

    return tempWidget;
}

void AProjectKapyGameMode::FlushInputs() const
{
    const auto Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    if (Controller != nullptr)
    {
        Controller->PlayerInput->FlushPressedKeys();
    }
}
