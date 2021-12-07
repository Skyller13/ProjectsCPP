// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CollectibleData.generated.h"

USTRUCT(BlueprintType)
struct PROJECTKAPY_API FCollectibleData
{
    GENERATED_BODY()

    public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectible")
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectible")
    int32 ID; //The ID of the object in the data table

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectible")
    UMaterial* ImageMaterial; //The icon of the object
};
