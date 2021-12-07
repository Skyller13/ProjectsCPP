// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumCustomMovement.generated.h"

UENUM(BlueprintType)
enum EmovementModeCustom
{
	CUSTOM_None		UMETA(DisplayName = "None"),

	CUSTOM_Climbing	UMETA(DisplayName = "Climbing"),

	CUSTOM_JumpClimbing UMETA(DisplayName = "JumpClimbing"),

	CUSTOM_Attack UMETA(DisplayName = "Attack"), //Melee attack

	CUSTOM_Aim UMETA(DisplayName = "Aim"), //Aim before shot 

	CUSTOM_Block UMETA(DisplayName = "Block"),

	CUSTOM_MAX		UMETA(Hidden),
};
