// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/NavLinkProxy.h"
#include "AINavLinkProxy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKAPY_API AAINavLinkProxy : public ANavLinkProxy
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	void SetSmartLinkData(const FVector& RelativeStart, const FVector& RelativeEnd, ENavLinkDirection::Type Direction);
	
};
