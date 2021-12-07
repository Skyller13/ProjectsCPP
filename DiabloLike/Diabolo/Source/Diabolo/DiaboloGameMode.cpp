// Copyright Epic Games, Inc. All Rights Reserved.

#include "DiaboloGameMode.h"
#include "DiaboloPlayerController.h"
#include "DiaboloCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

ADiaboloGameMode::ADiaboloGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ADiaboloPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}