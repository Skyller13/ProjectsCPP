// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "AntiVirusTacticalCharacter.generated.h"

class ULifeSystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBeginTurnSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndTurnSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSwitchTurnSignature);

UCLASS(Blueprintable)
class AAntiVirusTacticalCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AAntiVirusTacticalCharacter();

	// called at spawn
	virtual void BeginPlay() override;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool hasPlayed = false;
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 rangeToMoveOn = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 rangeAttack = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AGridNode* playerNode;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable, BlueprintCallable)
	FOnBeginTurnSignature OnBeginTurnEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable, BlueprintCallable)
	FOnEndTurnSignature OnEndTurnEvent;
	
	UPROPERTY(EditAnywhere,  BlueprintReadWrite, BlueprintAssignable, BlueprintCallable)
	FOnSwitchTurnSignature OnSwitchEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* LifeWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ULifeSystemComponent* lifeSystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	class UAnimMontage* AttackAnimation;

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

public:

	UFUNCTION(BlueprintCallable)
	void DisplayCharacterMovementRange();

	//Display Node pre-selection (under cursor)
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetLocationToTile_Event(AActor* _Grid, FVector _hitLocation);

	//hide Last node pre-selection (under cursor)
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void HideTile_Event(AActor* _Grid);

	//Check every node near the one player and highlight them (display movement range) 
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CheckNodeInRange_Event(const AGridNode* _playerNode);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SwitchBetweenCharacters();

	UFUNCTION(BlueprintCallable)
	bool Attack();
};

