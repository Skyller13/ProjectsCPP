// Fill out your copyright notice in the Description page of Project Settings.

#include "WallClimbable.h"
#include "ProjectKapyCharacter.h"

void AWallClimbable::Climbing(AProjectKapyCharacter* _character)
{
	if (_character != nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Climbing on %s"), *GetNameSafe(this)), true);
	}
}