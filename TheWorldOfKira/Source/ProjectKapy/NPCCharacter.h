// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Gameplay/Character/Component/QuestLogComponent.h"
#include "Gameplay/Interfaces/Interactible.h"
#include "Sound/DialogueTypes.h"


#include "NPCCharacter.generated.h"

UCLASS()
class PROJECTKAPY_API ANPCCharacter : public ACharacter, public IInteractible
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPCCharacter();

	UFUNCTION(BlueprintCallable)
	virtual void OnInteractionEnter() override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHaveToMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanBeFocus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector FinalLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector InitialPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display Quest")
	float rangeQuestDisplay = 7500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UQuestLogComponent* QuestLogPlayer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void WalkToDestination(FVector _location);

	virtual void CheckPlayerListenToConversation(float _DeltaTime);

	//this function set values of
	UFUNCTION(BlueprintImplementableEvent)
	void DisplayQuestWaypoint_Event();
	

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float pitchVoice = 1.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* dialogueTriggerArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isAbleToTalk = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* QuestMarkerLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UDialogueComponent* DialogueComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class ULifeSystemComponent* LifeSystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UToonShaderPointLightDetector* ShaderComponent;

	//
	UFUNCTION()
	void OnDialogueAreaOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnDialogueAreaEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void TalkToTarget_Event(AActor* Target);

	UFUNCTION()
	void EndDialogue(UDialogueComponent* Dialogue);
	
	UFUNCTION(BlueprintCallable)
	void SetNewFinalLocation(FVector _newLocation) { FinalLocation = _newLocation;}

	UFUNCTION(BlueprintCallable)
	void SetHaveToMove(bool _HaveToMove) { bHaveToMove = _HaveToMove; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class ULifeSystemComponent* GetLifeSystem() const { return LifeSystemComponent; }
};