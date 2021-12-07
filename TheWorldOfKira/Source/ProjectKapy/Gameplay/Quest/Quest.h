// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "ObjectivesData.h"
#include "QuestObjectiveType.h"
#include "QuestType.h"
#include "GameFramework/Actor.h"
#include "ProjectKapy/Gameplay/Actors/KillEnemiesArea.h"
#include "ProjectKapy/Gameplay/Actors/LocationActor.h"
#include "ProjectKapy/Graphic/DayNightCycleManager.h"

#include "Quest.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpawnTrigger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestDue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestFail);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObjectiveComplete, int32, index);

UCLASS()
class PROJECTKAPY_API AQuest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuest();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom")
	TArray<AActor*> ObjectsInWorld;

	///Quest Name
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	///Quest Overall description
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;
	
	///Quest Recap description
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Recap;

	///Quest location name 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LocationName;

	///Quest Contract type (Optional)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EQuestObjectiveType> ContractType;

	///Quest Objectives 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FObjectivesData> Objectives;

	///Quest Type (Main, Side, Ended, Repeatable, ...)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EQuestType> QuestType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WaitingText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString TurnInText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExperienceReward = 200.f;

	///Level required to be able to see the quest in the world
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LevelRequired = 1;

	///Does it requires a quest to be done first
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AQuest* PreRequisiteQuest = nullptr;

	///Who has gave you this quest
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* QuestGiver = nullptr;

	///To who you have to return the quest
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* TurnInActor = nullptr;

	///Is the quest complete
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsComplete = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRequireDayMoment = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<TYPEDayNightMoment> DayMoment;

	//Amount of skill points (reward)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 skillPointReward  = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 indexSkillUnlock = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture* textureSkill = nullptr;

	//Lasers and Waypoints
	UPROPERTY(VisibleAnywhere, Category = "Waypoints")
	TMap<int32, AActor*> Lasers;

	UPROPERTY(VisibleAnywhere, Category = "Waypoints")
	TMap<int32, AActor*> Waypoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoints")
	TSubclassOf<AActor> QuestLaserBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoints")
	TSubclassOf<AActor> QuestWaypointBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
	FOnSpawnTrigger OnSpawnTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
	FOnQuestDue OnQuestDue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
	FOnQuestFail OnQuestFail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
	FOnObjectiveComplete OnObjectiveComplete;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Other")
	bool bIsQuestFailed = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Other")
	bool bIsActive = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Other")
	bool bIsQuestEnded = false;

	UFUNCTION()
	void UpdateQuestObjectives(float _DeltaTime);

	UFUNCTION()
	void SpawnTriggerKillQuest();

	UFUNCTION()
	void SpawnTriggerObjectiveComplete(int32 _index);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Editor function that allow you to replace the quest in the world at the current parent location
	UFUNCTION(CallInEditor)
	virtual void OrganizeQuestInEditor();

	///Return true if the previous element is complete or if it does not require a previous Objective 
	UFUNCTION(BlueprintCallable)
	bool CheckPreviousObjective(TArray<FObjectivesData> _Objectives, int32 _indexObjective); 

	///Return true if the given index is valid and complete or if it does not require a specific Objective
	UFUNCTION(BlueprintCallable)
	bool CheckSpecificObjective(TArray<FObjectivesData> _Objectives, int32 _indexObjective, int32 _indexSpecificObjective);

	///Check if the player got the quest or not
	UFUNCTION(BlueprintCallable)
	virtual bool PlayerHasQuest(AQuest* _quest);

	//Events
	UFUNCTION(BlueprintCallable)
	virtual void CheckLocationObjective(ALocationActor* _LocationActor);

	UFUNCTION(BlueprintCallable)
	virtual void CheckNPCAtLocation(AActor* _actor, ALocationActor* _LocationActor);

	UFUNCTION(BlueprintCallable)
	virtual void CheckInteractionObjective(AActor* _InteractedActor);

	UFUNCTION(BlueprintCallable)
	virtual void CheckKilledEnemy(AActor* _enemy);

	UFUNCTION(BlueprintCallable)
	virtual void CheckIsPlayerListening(AActor* _actor, float _DeltaTime);

	UFUNCTION(BlueprintCallable)
	virtual void CheckEnemiesInArea(AKillEnemiesArea* _area);

	UFUNCTION(BlueprintCallable)
	virtual void CheckProtectedNPCIsAlive();

	UFUNCTION(BlueprintCallable)
	virtual void CheckScannedObjective(float _radius);

	UFUNCTION(BlueprintCallable)
	virtual void OnQuestActivate();

	UFUNCTION(BlueprintCallable)
	virtual void OnQuestDeactivate();

	//Set Objective Done (index = objective index)
	UFUNCTION(BlueprintCallable)
	virtual void CustomObjectiveComplete(int32 _ObjectiveIndex);

	//Call this function to update the UI
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateUI_Event();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UnlockSkill_Event();

	//Lasers & Markers
	///Delete every lasers and markers from the world
	UFUNCTION(BlueprintCallable)
    void ClearAllWaypoints();

	///Update the current position of markers and lasers in the world
	UFUNCTION(BlueprintCallable)
    void UpdateMarkersPosition();

	//Update 
	UFUNCTION(BlueprintCallable)
    void OnObjectiveCompleteUpdateWaypoints(int32 _index);

	///Create every markers and lasers for a quest 
	UFUNCTION(BlueprintCallable)
    void CreateMarkers();

	///Delete laser and marker of a specific objective
	UFUNCTION(BlueprintCallable)
    void RemoveMarkerAndWaypoint(int32 _ObjectiveIndex);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnQuestFailed_Event();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ObjectiveComplete_Event();

	UFUNCTION(BlueprintCallable)
	void SetQuestToFailState();

	UFUNCTION(BlueprintCallable)
	void QuestDueEvent();

	//Reset input velocity while checking a quest
	UFUNCTION(BlueprintCallable)
	void ResetFlush() const;
};