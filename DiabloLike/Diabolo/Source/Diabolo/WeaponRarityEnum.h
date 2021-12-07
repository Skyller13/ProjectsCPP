// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum WeaponRarityEnum
{
    Common UMETA(DisplayName = "Common"),
    Rare UMETA(DisplayName = "Rare"),
    Epic UMETA(DisplayName = "Epic"),
    Legendary UMETA(DisplayName = "Legendary"),
    Mythic UMETA(DisplayName = "Mythic"),
    Exalted UMETA(DisplayName = "Exalted"),    
};
