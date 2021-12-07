// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LevellingSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIABOLO_API ULevellingSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULevellingSystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int LevelCap = 40;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentExperience = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentMaxExperience = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentPercentage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BufferExperience = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float timeRemain = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> ExperienceWidget;

	UUserWidget* ExpUI;
	
	//Methods
	UFUNCTION(BlueprintCallable)
	void CalculatePercentage();

	UFUNCTION(BlueprintCallable)
	void CalculateMaxExperience(float _factorByLevel);

	UFUNCTION(BlueprintCallable)
	void AddExperience(float _value);

	UFUNCTION(BlueprintCallable)
	void LevelUp();
};
