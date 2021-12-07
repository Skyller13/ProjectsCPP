// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LevelSystemComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class PROJECTKAPY_API ULevelSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULevelSystemComponent();

	//properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LevelCap = 37;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentExperience = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentMaxExperience = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentPercentage = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BufferExperience = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> Experience_Widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* Experience_UI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float timeRemain = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor ExperienceBarColor = FLinearColor::Transparent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 skillNumberRemain = 0;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void CalculatePercentage();

	UFUNCTION(BlueprintCallable)
	void CalculateMaxExperience(float _expFactor);

	UFUNCTION(BlueprintCallable)
	void AddExperience(float _value, bool _bForceDisplay);

	UFUNCTION(BlueprintCallable)
	void AddSkillPoint(int32 _nbSkillPoint);

	UFUNCTION(BlueprintCallable)
	void LevelUp();

	UFUNCTION(BlueprintCallable)
	void DisplayExperienceBar(float _DeltaTime);

	UFUNCTION(BlueprintImplementableEvent)
	void AddExperience_EventUI(float _value);

	UFUNCTION(BlueprintImplementableEvent)
	void SetExperienceBarWidget_Event(UUserWidget* mainWidget);

	UFUNCTION(BlueprintImplementableEvent)
	void LevelUp_Event(UUserWidget* mainWidget);
};
