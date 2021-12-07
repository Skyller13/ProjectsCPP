// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Components/CanvasPanel.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectKapyGameMode.generated.h"

UCLASS(minimalapi)
class AProjectKapyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProjectKapyGameMode();

	//Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> MainWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> WaypointWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* MainWidgetUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCanvasPanel* CanvasPanel;

	//functions
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	UUserWidget* CreateWaypoint(AActor* _actor) const;
	
	UFUNCTION(BlueprintCallable)
	void FlushInputs() const;
};	