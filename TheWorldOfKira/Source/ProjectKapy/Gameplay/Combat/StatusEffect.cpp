// Fill out your copyright notice in the Description page of Project Settings.

#include "StatusEffect.h"
#include "LifeSystemComponent.h"
#include "EffectReceiverComponent.h"


AStatusEffect::AStatusEffect()
{
	PrimaryActorTick.bCanEverTick = false;
	bIsCompletedByDuration = true;
}

void AStatusEffect::Init(UEffectReceiverComponent* Receiver)
{
	ReceiverComponent = Receiver;
	Character = Cast<AProjectKapyCharacter>(Receiver->GetOwner());
	ElapsedTimeToProcess = 0.f;
	ElapsedTime = 0.f;
 
	if (AttachToOwner)
	{
		SetActorLocation(FVector::ZeroVector);
		SetActorRotation(FRotator::ZeroRotator);
		AttachToActor(Receiver->GetOwner(), FAttachmentTransformRules::KeepRelativeTransform);
	}

	InitEffect();
}

void AStatusEffect::InitEffect()
{
	InitEffect_BP();
}

void AStatusEffect::EndEffect()
{
	EndEffect_BP();
	OnEffectCompleted.Broadcast();
}

void AStatusEffect::ProcessEffect()
{
	ProcessEffect_BP();
}

void AStatusEffect::TickProcess(float dt)
{
	ElapsedTimeToProcess += dt;
	ElapsedTime += dt;

	TickProcess_BP(dt);

	if (ElapsedTimeToProcess >= TimeToProcess)
	{
		Process();
	}
}

void AStatusEffect::Process()
{
	ElapsedTimeToProcess -= TimeToProcess;
	ProcessEffect();
}

bool AStatusEffect::IsCompleted()
{
	if (bIsCompletedByDuration)
	{
		return ElapsedTime >= Duration;
	}

	return false;
}

void AStatusEffect::Clear()
{
	EndEffect();
}
