// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ButtonBehaviour.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class PROJECTKAPY_API UButtonBehaviour : public UObject
{
	GENERATED_BODY()

public:

	UButtonBehaviour();

protected:
	virtual void Execute();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_Execute();
public:
	UFUNCTION(BlueprintCallable)
	void RequestExecution();
};
