// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySpell.h"


#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectKapy/ProjectKapyCharacter.h"
#include "ProjectKapy/Gameplay/Combat/EffectReceiverComponent.h"
#include "ProjectKapy/Gameplay/Combat/StatusEffect.h"

UAbilitySpell::UAbilitySpell()
{
	ActionName = "Spell";
	bInCooldown = false;
	bForceEffectDestroy = true;
}

void UAbilitySpell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsExecuting)
	{
		if (ExecutingElapsedTime >= CastTime)
		{
			ApplyEffect();
			Interrupt();
		}
		else if (Character->CharacterState != STATE_Cast)
		{
			Interrupt();
		}
	}
	else if (bEffectInProgress)
	{
		EffectTimer += DeltaTime;
		if (EffectTimer >= EffectTime)
		{
			StartCooldown();
		}
	}
	else if (bInCooldown)
	{
		CooldownTimer += DeltaTime;
		if (CooldownTimer >= Cooldown)
		{
			OnSpellEndCooldown.Broadcast();
			bInCooldown = false;
		}
	}
}

void UAbilitySpell::AbilityPressed()
{
	Super::AbilityPressed();
	if (IsExecutionValid())
	{
		Execute();
	}
}

void UAbilitySpell::AbilityRelease()
{
	Super::AbilityRelease();
}

void UAbilitySpell::Interrupt()
{
	Super::Interrupt();
	if (Character->CharacterState == STATE_Cast)
		Character->SetCharacterState(ECharacterState::STATE_None);

	OnSpellEndCasting.Broadcast();
	if (EffectInstancied != nullptr && bForceEffectDestroy)
	{
		Character->EffectReceiver->RemoveEffect(EffectInstancied);
	}
}

void UAbilitySpell::Execute()
{
	Super::Execute();
	Character->SetCharacterState(ECharacterState::STATE_Cast);
	OnSpellStartCasting.Broadcast();
}

void UAbilitySpell::ApplyEffect()
{
	if (StatusEffect)
	{
		EffectInstancied = GetWorld()->SpawnActor<AStatusEffect>(StatusEffect, Character->GetActorLocation(),
		                                                         Character->GetActorRotation());

		if (EffectInstancied)
		{
			EffectInstancied->PowerModifier = PowerModifier;
			EffectInstancied->Duration *= DurationModifier;

			Character->EffectReceiver->AddEffect(EffectInstancied);

			EffectTime = EffectInstancied->Duration;
			bEffectInProgress = true;
		}
	}
}

void UAbilitySpell::StartCooldown()
{
	bEffectInProgress = false;
	CooldownTimer = 0.f;
	bInCooldown = true;
	OnSpellStartCooldown.Broadcast();
}

bool UAbilitySpell::IsExecutionValid() const
{
	return Super::IsExecutionValid() && !bInCooldown && !bEffectInProgress &&
		Character->CharacterState == STATE_None;
}

float UAbilitySpell::GetCastChargePercent() const
{
	if (CastTime != 0.f)
	{
		return ExecutingElapsedTime / CastTime;
	}
	return 0.f;
}

float UAbilitySpell::GetCooldownPercent() const
{
	if (Cooldown != 0.f)
	{
		return CooldownTimer / Cooldown;
	}
	return 0.f;
}

void UAbilitySpell::SetStatusEffect(TSubclassOf<AStatusEffect> NewEffect)
{
	StatusEffect = NewEffect;
}

void UAbilitySpell::OnCharacterReceiveDamage(AActor* DamagedActor, float Value, AActor* DamageCauser)
{
	if (bIsExecuting)
	{
		Interrupt();
	}
}
