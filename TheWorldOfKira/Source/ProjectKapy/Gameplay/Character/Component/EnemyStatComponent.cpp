// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyStatComponent.h"


#include "LevelSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectKapy/KiraCharacter.h"
#include "ProjectKapy/Gameplay/Abilities/AttackAbility.h"
#include "ProjectKapy/Gameplay/Abilities/ProjectileTemplate.h"
#include "ProjectKapy/Gameplay/Abilities/ShootAbility.h"
#include "ProjectKapy/Gameplay/Combat/LifeSystemComponent.h"
#include "ProjectKapy/Graphic/DayNightCycleManager.h"

// Sets default values for this component's properties
UEnemyStatComponent::UEnemyStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	auto player = Cast<AKiraCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (player && player->GetLevelSystemComp())
	{
		const int32 playerLevel = player->GetLevelSystemComp()->CurrentLevel;

		//Random level with range given
		Level = FMath::RandRange((playerLevel - randomMin), (playerLevel + randomMax));

		if (IsBuff)
			Level += randomMin;

		//Constraint min and max level !
		if (Level <= 0)
			Level = 1;

		if (Level > 37)
			Level = 37;

		float buffFactor = 0.f;

		if (IsBuff)
			buffFactor = 0.25f;

		//Set experience reward
		if (ExperienceCurve)
			experienceReward = ceil(ExperienceCurve->GetFloatValue(Level) * (1.f + buffFactor));
	}

	ComputeAttackDamage();
	ComputeShootDamage();
	ComputeLife();
}

// Called every frame
void UEnemyStatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEnemyStatComponent::ComputeAttackDamage()
{
	UAbilityManagerComponent* AbilityMng = GetOwner()->FindComponentByClass<UAbilityManagerComponent>();
	if (AbilityMng != nullptr)
	{
		float Factor = GetAttackStats();

		UAttackAbility* AtkAbility = AbilityMng->GetAbilityByClass<UAttackAbility>();
		if (AtkAbility != nullptr)
		{
			AtkAbility->SetAttackModifier(Factor);
		}
	}
}

void UEnemyStatComponent::ComputeShootDamage()
{
	UAbilityManagerComponent* AbilityMng = GetOwner()->FindComponentByClass<UAbilityManagerComponent>();
	if ( AbilityMng != nullptr)
	{
		float Factor = GetShootStats();

		UShootAbilityBase* ShootAbility = AbilityMng->GetAbilityByClass<UShootAbilityBase>();
		if (ShootAbility != nullptr)
		{
			UProjectileTemplate* Template = ShootAbility->GetMainProjectileTemplate();
			if (Template)
			{
				Template->Modifier.Damage = Factor;
				Template->SetModifier(Template->Modifier);
			}
		}
	}
}

void UEnemyStatComponent::ComputeLife()
{
	ULifeSystemComponent* LifeSystem = GetOwner()->FindComponentByClass<ULifeSystemComponent>();
	if (LifeSystem)
	{
		LifeSystem->SetHPMax(GetHPStats());
	}
}

float UEnemyStatComponent::GetAttackStats() const
{
	if (AttackCurve)
	{
		return AttackCurve->GetFloatValue(Level);
	}
	return AttackBase;
}

float UEnemyStatComponent::GetShootStats() const
{
	if (ShootCurve)
	{
		return ShootCurve->GetFloatValue(Level);
	}
	return ShootBase;
}

float UEnemyStatComponent::GetHPStats() const
{
	if (HealCurve)
	{
		return HealCurve->GetFloatValue(Level);
	}
	return HPBase;
}
