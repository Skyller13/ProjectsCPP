// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectKapy/Gameplay/Abilities/AbilitySpell.h"
#include "AbilitySpellHold.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKAPY_API UAbilitySpellHold : public UAbilitySpell
{
	GENERATED_BODY()

	protected:
	
	public:
	UAbilitySpellHold();
	virtual void AbilityRelease() override;
	virtual void Execute() override;
	virtual  void Tick(float DeltaTime) override;
	virtual  void Interrupt() override;
};
