// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectKapy/Gameplay/Quest/Quest.h"

#include "QuestLogComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class PROJECTKAPY_API UQuestLogComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestLogComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	AQuest* currentQuest = nullptr;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Quest log ui reference
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UUserWidget* QuestLogUI;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> QuestLogWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AQuest* ActiveQuest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AQuest*> Quests;

	//Quest log
	UFUNCTION(BlueprintCallable)
	void AddQuestToLog(AQuest* _Quest);

	UFUNCTION(BlueprintCallable)
	void ToggleQuestLog();

	UFUNCTION(BlueprintCallable)
	void RemoveFromQuestLog(AQuest* _Quest);

	///Set Quest modifications (UI and activity)
	UFUNCTION(BlueprintCallable)
	void OnActiveQuestChanged(AQuest* _NewQuest);
};
