// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Climbable.h"
#include "WallClimbable.generated.h"

class AProjectKapyCharacter;

UCLASS()
class PROJECTKAPY_API AWallClimbable : public AClimbable
{
	GENERATED_BODY()
	
public : 
	virtual void Climbing(AProjectKapyCharacter* _character) override;
};
