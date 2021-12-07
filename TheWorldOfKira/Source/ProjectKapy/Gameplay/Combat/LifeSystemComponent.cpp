// Fill out your copyright notice in the Description page of Project Settings.


#include "LifeSystemComponent.h"

#include "ProjectKapy/ProjectKapyCharacter.h"

// Sets default values for this component's properties
ULifeSystemComponent::ULifeSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	HpBase = 50.f;
	HpCurrent = HpBase;
	Armor = 0.f;
	CanReceiveDamage = true;
	CanReceiveHeal = true;
	CanDie = true;
	bAlreadyDead = false;
}

// Called when the game starts
void ULifeSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	HpCurrent = HpBase;
	Character = Cast<ACharacter>(GetOwner());
}


// Called every frame
void ULifeSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!bAlreadyDead)
	{
		if (HpCurrent <= 0.f && CanDie)
		{
			Death();
		}
		else
		{
			ElaspedTimeWithoutHit += DeltaTime;
			if (ElaspedTimeWithoutHit >= TimeWithoutHitForRegen)
			{
				HpCurrent += LifeRegenerationRate * DeltaTime;
				HpCurrent = FMath::Clamp(HpCurrent, 0.f, HpBase);
			}
		}
	}
}

void ULifeSystemComponent::ApplyHealToHP(float _heal)
{
	if (!CanReceiveHeal || bAlreadyDead)
		return;

	HpCurrent += _heal;
	HpCurrent = FMath::Clamp(HpCurrent, 0.f, HpBase);

	OnHealReceived.Broadcast(_heal);
}


float ULifeSystemComponent::GetHealthPercent() const
{
	if (HpBase != 0.f)
		return HpCurrent / HpBase;
	return 0.f;
}

void ULifeSystemComponent::EndDeathAnimBroadcast()
{
	OnDeathEndAnim.Broadcast();
}

void ULifeSystemComponent::Revive()
{
	if (!bAlreadyDead)
		return;

	bAlreadyDead = false;
	HpCurrent = HpBase;
	Armor = 0.f;
	OnRevive.Broadcast();
}

void ULifeSystemComponent::Death()
{
	if (!Character)
	{
		return;
	}

	if (!bAlreadyDead)
	{
		OnDeath.Broadcast();
		bAlreadyDead = true;
	}
}

//Category Damage

void ULifeSystemComponent::ApplyDamageToHP(float _Damage, AActor* Causer, ECharacterType TypeCauser,
                                           EDamageType TypeDamage)
{
	if (!CanReceiveDamage || bAlreadyDead)
		return;

	if (IsDamageSensible(TypeCauser))
	{
		float damageValue = _Damage;

		switch(TypeDamage) {
		case DT_Distance:
			damageValue *= DistanceDamageModifier;
			break;
		case DT_Melee:
			damageValue *= MeleeDamageModifier;
			break;
		default:
			break;
		}
		
		damageValue = FMath::Clamp(damageValue - Armor, 0.f, damageValue);
		HpCurrent -= damageValue;
		HpCurrent = FMath::Clamp(HpCurrent, 0.f, HpBase);
		ElaspedTimeWithoutHit = 0.f;
		OnDamageReceived.Broadcast(GetOwner(), damageValue, Causer, TypeDamage);
	}
}

void ULifeSystemComponent::ApplyDamageAtLocalPos(float _Damage, FVector _LocalPos, AActor* Causer,
                                                 ECharacterType TypeCauser, EDamageType TypeDamage)
{
	ApplyDamageToHP(_Damage, Causer, TypeCauser, TypeDamage);
	if (Character == nullptr)
		return;

	if (Character->GetCurrentMontage() == nullptr)
	{
		float angle = FMath::Atan2(_LocalPos.Y, _LocalPos.X);
		angle = FMath::RadiansToDegrees(angle);
		PlayAnimHitFromAngle(angle);
	}
}

void ULifeSystemComponent::ApplyDamageAtPos(float _Damage, FVector _WorldPos, AActor* Causer, ECharacterType TypeCauser,
                                            EDamageType TypeDamage)
{
	if (Character == nullptr)
	{
		ApplyDamageToHP(_Damage, Causer, TypeCauser, TypeDamage);
		return;
	}

	ApplyDamageAtLocalPos(_Damage, _WorldPos - Character->GetActorLocation(), Causer, TypeCauser, TypeDamage);
}

void ULifeSystemComponent::PlayAnimHitFromAngle(float _AngleDeg)
{
	if (Character == nullptr)
		return;

	if (BackHitAnim != nullptr && (_AngleDeg < -135.f || _AngleDeg > 135.f))
	{
		Character->PlayAnimMontage(BackHitAnim);
	}
	else if (LeftHitAnim != nullptr && (_AngleDeg > -135.f && _AngleDeg < -45.f))
	{
		Character->PlayAnimMontage(LeftHitAnim);
	}
	else if (RightHitAnim != nullptr && (_AngleDeg < 135.f && _AngleDeg > 45.f))
	{
		Character->PlayAnimMontage(RightHitAnim);
	}
	else if (FrontHitAnim != nullptr)
	{
		Character->PlayAnimMontage(FrontHitAnim);
	}
}

bool ULifeSystemComponent::IsAlive() const
{ 
	if (bAlreadyDead)
	{
		return false;
	}

	if (HpCurrent > 0)
	{
		return true;
	}

	return false;
}

void ULifeSystemComponent::SetHPMax(float HPMax)
{
	HpBase = HPMax;
	HpCurrent = HpBase;
}

bool ULifeSystemComponent::IsDamageSensible(ECharacterType _type)
{
	if (_type == ECharacterType::CT_None)
	{
		return true;
	}

	for (ECharacterType ct : SensitiveType)
	{
		if (ct == _type)
			return true;
	}
	return false;
}
