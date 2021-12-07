// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySpellHold.h"

#include "ProjectKapy/ProjectKapyCharacter.h"

UAbilitySpellHold::UAbilitySpellHold()
{
}

void UAbilitySpellHold::AbilityRelease()
{
	Super::AbilityRelease();
	if (bIsExecuting)
	{
		Interrupt();
	}
}

void UAbilitySpellHold::Execute()
{
	Super::Execute();
	ApplyEffect();
}

void UAbilitySpellHold::Tick(float DeltaTime)
{
	UAbilityBase::Tick(DeltaTime);
	if (!bIsExecuting && bInCooldown)
	{
		CooldownTimer += DeltaTime;
		if (CooldownTimer >= Cooldown)
		{
			bInCooldown = false;
			OnSpellEndCooldown.Broadcast();
		}
	}

	if (bIsExecuting)
	{
		if (ExecutingElapsedTime >= CastTime)
		{
			Interrupt();
		}
		else if (Character->CharacterState != STATE_Cast)
		{
			Interrupt();
		}
	}
}

void UAbilitySpellHold::Interrupt()
{
	Super::Interrupt();
	if (!bInCooldown)
	{
		StartCooldown();
	}
}
