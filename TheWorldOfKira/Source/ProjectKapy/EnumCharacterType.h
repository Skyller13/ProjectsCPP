#pragma once

#include "CoreMinimal.h"
#include "EnumCharacterType.generated.h"

UENUM(BlueprintType, Blueprintable)
enum ECharacterType
{
	CT_None = 0 UMETA(DisplayName = "None"),
    CT_Player UMETA(DisplayName = "Player"),
    CT_Enemy UMETA(DisplayName = "Enemy"),
    CT_NPC UMETA(DisplayName = "NPC"),
    CT_Max UMETA(DisplayName = "Max")
};