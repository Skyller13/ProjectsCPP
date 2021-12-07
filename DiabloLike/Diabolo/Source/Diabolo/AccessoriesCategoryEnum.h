// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum AccessoriesCategoryEnum
{
	Scope UMETA(DisplayName = "Scope"),
    Grip UMETA(DisplayName = "Grip"),
    Silencer UMETA(DisplayName = "Silencer"),
    Magazine UMETA(DisplayName = "Magazine"),
    Bullet UMETA(DisplayName = "Bullet"),
};
