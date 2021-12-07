// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


#include "AntiVirusTacticalPlayerController.h"
#include "VirusCharacter.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "AntiVirusTacticalGameMode.generated.h"

UCLASS(minimalapi)
class AAntiVirusTacticalGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
private:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere)
	AActor* grid;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> GridActor;

	//Turn
	UPROPERTY(EditAnywhere)
	bool isPlayerTurn = true;

	//Max number of enemies at the same time in the world
	UPROPERTY(EditAnywhere)
	int32 maxEnemies = 10;

	UPROPERTY()
	AAntiVirusTacticalPlayerController* Controller;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AVirusCharacter*> Enemies;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> MainWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* MainWidgetUI = nullptr;
	
	AAntiVirusTacticalGameMode();

	UFUNCTION(BlueprintCallable)
	AActor* GetGrid() const {return grid;}

	UFUNCTION(BlueprintCallable)
	void ChangeTurn();

	UFUNCTION(BlueprintCallable)
	bool GetTurn() const {return isPlayerTurn;}

	UFUNCTION()
	void OnEnemiesEndTurn();

	UFUNCTION()
	int32 GetMaxEnemies() const {return maxEnemies;}

	UFUNCTION()
	void WaitUntilEnemyStartPlaying();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnWinEvent();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnLoseEvent();
};