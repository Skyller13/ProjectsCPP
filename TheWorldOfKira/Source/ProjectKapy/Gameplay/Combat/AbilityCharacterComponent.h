// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectKapy/ProjectKapyCharacter.h"
#include "AbilityCharacterComponent.generated.h"


class AProjectKapyCharacter;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTKAPY_API UAbilityCharacterComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	bool bIsPress;

	float PressElapsedTime;

	UPROPERTY()
	AProjectKapyCharacter* Character;

public:
	// Sets default values for this component's properties
	UAbilityCharacterComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	virtual void AbilityPress();
	UFUNCTION()
	virtual void AbilityRelease();
	
	UFUNCTION(BlueprintCallable, Category = Ability)
	virtual void Interrupt();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AProjectKapyCharacter* GetCharacter() const { return Character; }

	// UFUNCTION()
	// virtual bool ValidState(ECharacterState newState);
	// UFUNCTION()
	// virtual void ResetState(ECharacterState previousState);
	// UFUNCTION()
	// virtual void InitNewState(ECharacterState newState);
};
