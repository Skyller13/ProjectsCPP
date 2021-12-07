// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(Blueprintable)
enum PlayerStateEnum
{
  Dead UMETA(DisplayName = "Death"),
  Alive UMETA(DisplayName = "Alive"),
  Injured UMETA(DisplayName = "Injured"),
};
