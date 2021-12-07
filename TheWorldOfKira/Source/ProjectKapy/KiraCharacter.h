// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectKapyCharacter.h"
#include "Sound/DialogueVoice.h"


#include "KiraCharacter.generated.h"


class ULevelSystemComponent;
class UQuestLogComponent;
/**
 * 
 */
UCLASS()
class PROJECTKAPY_API AKiraCharacter : public AProjectKapyCharacter
{
	GENERATED_BODY()


protected:

	UPROPERTY(VisibleAnywhere)
	ULevelSystemComponent* LevelSystemComponent;
	UPROPERTY(VisibleAnywhere)
	TMap<int32, int32> ItemsCollected; //inventory items

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UTargetingComponent* TargetingComponent;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int presses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isSpeakingToANPC = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UQuestLogComponent* QuestLog;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Target;

public:
	AKiraCharacter();

protected:

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	* Called via input to turn at a given rate. 
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to turn look up/down at a given rate. 
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);

	UFUNCTION(BlueprintCallable)
	void AimPressed(FKey key);

	UFUNCTION(BlueprintCallable)
	void AimRelease(FKey key);

	UFUNCTION()
	void DoubleJump(FKey key);

	UFUNCTION()
	void JumpWhileClimb(FKey key);

	UFUNCTION()
	void Climb(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	           FVector NormalImpulse, const FHitResult& hit);

	UFUNCTION()
	void Descent(FKey key);
	UFUNCTION()
	void AttackPressed(FKey key);
	UFUNCTION()
	void AttackRelease(FKey key);
	UFUNCTION()
	void FirePressed(FKey key);
	UFUNCTION()
	void FireRelease(FKey key);
	UFUNCTION()
	void BlockPressed(FKey key);
	UFUNCTION()
	void BlockRelease(FKey key);
	UFUNCTION()
	void SpellPressed(FKey key);
	UFUNCTION()
	void SpellRelease(FKey key);

	void AutoSelectQuest();
public:

	virtual void BeginPlay() override;
	virtual void Tick(float Deltatime) override;

	void CheckCollectionObjective();
	UFUNCTION(BlueprintCallable)
	ULevelSystemComponent* GetLevelSystemComp();

	UFUNCTION(BlueprintCallable)
	TMap<int32, int32> GetItemsCollected();

	UFUNCTION(BlueprintCallable)
	void AddItemToCollection(int32 _index, int32 _value);

	UFUNCTION(BlueprintCallable)
	void SwitchQuest();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UTargetingComponent* GetTargetingComponent() const { return TargetingComponent; }

	UFUNCTION(BlueprintImplementableEvent)
	bool BP_CanInteract();
};
