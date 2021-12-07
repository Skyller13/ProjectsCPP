// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "AntiVirusTacticalCharacter.h"
#include "GameFramework/PlayerController.h"
#include "AntiVirusTacticalPlayerController.generated.h"

UENUM(BlueprintType)
enum ECurrentAction
{
	Walk,
	Attack,
	Wait
};

UCLASS()
class AAntiVirusTacticalPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAntiVirusTacticalPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AGridNode* currentSelection = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ECurrentAction> CurrentAction;
	
	/** Navigate player to the given world location. */
	UFUNCTION(BlueprintCallable)
	void SetPlayerToMoveOnSelectedNode();

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetPawns() const {return PawnList;}

	UFUNCTION(BlueprintCallable)
	bool GetIsPlayerTurn() const { return isPlayerTurn; }

	//Remove pawn from list when he died 
	UFUNCTION()
    void RemovePawnFromList(AActor* _Pawn);

	//Change character
	UFUNCTION(BlueprintCallable)
    void OnSwitchCharacter();

	UFUNCTION(BlueprintCallable)
	bool CheckNbCharacterRemains() const;
	
protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	UPROPERTY()
	int32 currentIndexPawn = 0;

	UPROPERTY()
	bool isPlayerTurn = true;

	//List of every actors in your team
	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> PawnList;

	// Begin PlayerController interface
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	//Change current selection on the grid (node)
	void SelectTileUnderMouseCursor();

	//Set the variable : Current Selection that give the selected node in the world
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ChangeNodeSelection_Event(const FVector NodeLocation);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	float GetPathToNode_Event(const AGridNode* _startNode, const FVector _hitLocation);
	
	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();

	//Change Character Event (Camera effect)
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnPossessSwitchCamera_Event(AAntiVirusTacticalCharacter* _newPlayer);
};