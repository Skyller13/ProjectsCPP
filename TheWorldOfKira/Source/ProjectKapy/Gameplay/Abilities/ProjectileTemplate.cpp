// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileTemplate.h"
#include "AbilityActor/Projectile.h"

UProjectileTemplate::UProjectileTemplate()
{
}

void UProjectileTemplate::SetModifier(FProjectileModifier NewModifier)
{
	Modifier = NewModifier;
	ComputeProperties();
}

void UProjectileTemplate::ComputeProperties()
{
	ComputedProperties = BaseProperties;
	ComputedProperties.DamageDirectHit *= Modifier.Damage;
	ComputedProperties.DamageExplosion *= Modifier.Damage;

	ComputedProperties.RadiusExplosion *= Modifier.RangeExplosion;

	CastTime = CastTimeBase * Modifier.Speed;
	RecoveryTime = RecoveryTimeBase * Modifier.Speed;

	ManaCost = ManaCostBase * Modifier.Mana;

	if (ComputedProperties.bUseGravity)
	{
		ComputedProperties.GravityOverride *= Modifier.Gravity;
	}

	ComputedProperties.DistanceMax *= Modifier.Range;
	ComputedProperties.DeathTime *= Modifier.Range;
	
	ComputedProperties.LaunchSpeed *= Modifier.Velocity;

	if (FiringAnim)
	{
		for (FAnimNotifyEvent Notify : FiringAnim->Notifies)
		{
			if (Notify.NotifyName == "LaunchProjectile")
			{
				AnimNotifTime = Notify.GetTime() / AnimRate;
			}
		}
	}
}

bool UProjectileTemplate::IsValid() const
{
	return ProjectileActor != nullptr;
}
