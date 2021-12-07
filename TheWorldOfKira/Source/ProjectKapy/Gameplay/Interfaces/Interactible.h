// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactible.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class PROJECTKAPY_API UInteractible : public UInterface
{
    GENERATED_BODY()
};

/**
 * 
 */
class PROJECTKAPY_API IInteractible
{
    GENERATED_BODY()

public:
    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
    virtual void OnInteractionEnter();
};
