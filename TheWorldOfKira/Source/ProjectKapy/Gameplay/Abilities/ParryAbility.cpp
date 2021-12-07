// Fill out your copyright notice in the Description page of Project Settings.


#include "ParryAbility.h"

#include "AbilityActor/Projectile.h"
#include "ProjectKapy/ProjectKapyCharacter.h"
#include "ProjectKapy/Gameplay/Combat/EffectReceiverComponent.h"
#include "ProjectKapy/Gameplay/Combat/StatusEffect.h"

UParryAbility::UParryAbility()
{
	ActionName = "Block";
}

void UParryAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsExecuting)
	{
		ParryElapsedTime += DeltaTime;
	}
}

void UParryAbility::AbilityPressed()
{
	Super::AbilityPressed();
	Execute();
}

void UParryAbility::AbilityRelease()
{
	Super::AbilityRelease();
	Interrupt();
}

void UParryAbility::Interrupt()
{
	Super::Interrupt();
	ParryCount = 0;
	Character->SetCharacterState(ECharacterState::STATE_None);
}

void UParryAbility::Execute()
{
	Super::Execute();
	ParryElapsedTime = 0.f;
	Character->SetCharacterState(ECharacterState::STATE_Parade);
}

bool UParryAbility::BlockAttack(FParryResult& ParryResult, UAttackAsset* Attack,
                                UPrimitiveComponent* OverlappedComponent, FVector InitiatorLocation)
{
	if (!bIsExecuting)
		return false;

	FVector ToInitiator = InitiatorLocation - Character->GetActorLocation();
	float Dot = FVector::DotProduct(Character->GetActorForwardVector(),
	                                ToInitiator.GetSafeNormal());
	UE_LOG(LogTemp, Warning, TEXT("%f"),  Dot);
	if (Dot <= 0.f)
	{
		return false;
	}

	Character->PlayAnimMontage(AnimReact, 1.f);
	if (ParryElapsedTime < ParryPerfectTime && bCanParryPerfectAttack)
	{
		ParryResult.bIsPerfectParry = true;
		BP_OnPerfectAttackParry();
		AddBuff();
	}

	return true;
}

bool UParryAbility::BlockProjectile(FParryResult& ParryResult, AProjectile* Projectile,
                                    UPrimitiveComponent* OverlappedComponent)
{
	if (!bIsExecuting)
		return false;

	if (Projectile == nullptr)
	{
		return true;
	}

	float Dot = FVector::DotProduct(Character->GetActorForwardVector(),
	                                Projectile->GetVelocity());

	if (Dot >= 0.f)
	{
		return false;
	}

	Character->PlayAnimMontage(AnimReact, 1.f);
	if (ParryElapsedTime < ParryPerfectTime && bCanParryPerfectShoot)
	{
		ParryResult.bIsPerfectParry = true;
		ReflectProjectile(Projectile);
	}
	return true;
}

void UParryAbility::ReflectProjectile(AProjectile* Projectile)
{
	if (Projectile->ReflectProjectile)
	{
		AProjectile* ProjectileInstancied = AbilityManager->GetWorld()->SpawnActorDeferred<AProjectile>(
			Projectile->ReflectProjectile, Projectile->GetActorTransform(), Character, Character);
		ProjectileInstancied->Init(this);
		ProjectileInstancied->OverrideProperties(Projectile->Properties);
		ProjectileInstancied->FinishSpawning(Projectile->GetActorTransform());


		FVector vBounce = -2.f * FVector::DotProduct(Projectile->GetVelocity(), Character->GetActorForwardVector())
		                                            * Character->GetActorForwardVector() + Projectile->GetVelocity();

		ProjectileInstancied->SetupProjectile(vBounce);
	}
}

void UParryAbility::AddBuff()
{
	if (BuffInstancied != nullptr)
	{
		Character->EffectReceiver->RemoveEffect(BuffInstancied);
	}
	
	if (EffectBuff)
	{
		BuffInstancied = Character->EffectReceiver->AddEffectByClass(EffectBuff);
		if (BuffInstancied)
		{
			BuffInstancied->OnEffectCompleted.AddDynamic(this, &UParryAbility::ClearBuff);
		}
	}
}

void UParryAbility::ClearBuff()
{
	BuffInstancied = nullptr;
}
