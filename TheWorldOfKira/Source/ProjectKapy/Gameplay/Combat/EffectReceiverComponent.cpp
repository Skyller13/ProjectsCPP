// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectReceiverComponent.h"

#include "LifeSystemComponent.h"
#include "StatusEffect.h"

// Sets default values for this component's properties
UEffectReceiverComponent::UEffectReceiverComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEffectReceiverComponent::BeginPlay()
{
	Super::BeginPlay();
	ULifeSystemComponent* LifeSystem = GetOwner()->FindComponentByClass<ULifeSystemComponent>();
	if (LifeSystem)
	{
		LifeSystem->OnRevive.AddDynamic(this, &UEffectReceiverComponent::ClearEffects);
	}
}

// Called every frame
void UEffectReceiverComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (int32 i = 0; i != StatusEffectActive.Num(); ++i)
	{
		AStatusEffect* effect = StatusEffectActive[i];
		effect->TickProcess(DeltaTime);
		if (effect->IsCompleted())
		{
			StatusEffectActive[i] = nullptr;
			effect->Clear();
			effect->Destroy();
		}
	}

	StatusEffectActive.RemoveAll([](const AStatusEffect* Ptr)
	{
		return Ptr == nullptr;
	});

	// ...
}

void UEffectReceiverComponent::AddEffect(AStatusEffect* Effect)
{
	if (Effect)
	{
		StatusEffectActive.Add(Effect);
		Effect->Init(this);
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AddEffect"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Error Can't Add Effect NULL"));
	}
}

AStatusEffect* UEffectReceiverComponent::AddEffectByClass(TSubclassOf<AStatusEffect> NewEffect)
{
	if (!NewEffect)
		return nullptr;

	AStatusEffect* newEffect = GetWorld()->SpawnActor<AStatusEffect>(NewEffect, GetOwner()->GetActorLocation(),
	                                                                 GetOwner()->GetActorRotation());

	AddEffect(newEffect);
	return newEffect;
}

void UEffectReceiverComponent::ClearEffects()
{
	for (int32 i = 0; i != StatusEffectActive.Num(); ++i)
	{
		AStatusEffect* effect = StatusEffectActive[i];
		StatusEffectActive[i] = nullptr;
		effect->Clear();
		effect->Destroy();
	}

	StatusEffectActive.RemoveAll([](const AStatusEffect* Ptr)
	{
		return Ptr == nullptr;
	});
}

void UEffectReceiverComponent::RemoveEffect(AStatusEffect* Effect)
{
	if (StatusEffectActive.Contains(Effect))
	{
		StatusEffectActive.Remove(Effect);
	}

	Effect->Clear();
	Effect->Destroy();
}

void UEffectReceiverComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);
	ClearEffects();
}
