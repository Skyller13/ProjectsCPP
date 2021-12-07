// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ObjectivesType.h"
#include "ObjectivesData.generated.h"

USTRUCT(BlueprintType)
struct PROJECTKAPY_API FObjectivesData
{
    GENERATED_BODY();

public:
    //objective description
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description; 

    //objective type (enum)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TEnumAsByte<EObjectivesType> Type;

    //the current target in the world
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    AActor* Target; 

    //The actors to spawn in the world
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<TSubclassOf<AActor>, int32> SpawnerList; 

    //the list of actors spawned in the world
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<AActor*> TargetList; 

    //is the objective done
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsComplete;

    //is the objective failed
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsFailed;

    //Number of entity/enemy required to complete the objective (depends on the objective type)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Number; 

    ///Current kill count
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentKillCount = 0;

    //Number (index) of the item in the data table  
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 IDItem; 

    // the current timer value
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentTimer; 

    //if this value is greater than 0, the objective will be over time (if you fail it, you'll have to restart the objective from the last checkpoint)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxTimer;

    //Does the objective require the previous one to be visible ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bRequirePreviousObjective; 

    //Does the objective require a specific objective to be done ? 
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bRequireSpecificObjective; 

    //If it requires a specific objective to be done, then check this index !
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 indexSpecificObjective;

    //Is the objective optional to complete the quest ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsOptional;

    //The backup of the target list for the reset in case of fail ! (Do not add anything in editor)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Backup")
    TArray<AActor*> TargetListBackup; 
};
