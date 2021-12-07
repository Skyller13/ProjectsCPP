// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DiaboloPlayerController.generated.h"

UCLASS()
class ADiaboloPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ADiaboloPlayerController();

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void BeginPlay() override;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	// End PlayerController interface
	
	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	void OnSkill0Requested();
	void OnSkill1Requested();
	void OnSkill2Requested();

public :
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> MainWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> ExpWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> floatingWidget;
	
	UUserWidget* MainUI;

	UUserWidget* ExperienceUI;

	UUserWidget* HealthUI;

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFloatingDamage(float Value, FLinearColor Color, FVector ActorLocation);
};


