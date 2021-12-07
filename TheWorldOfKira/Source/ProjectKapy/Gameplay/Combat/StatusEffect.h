// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "StatusEffect.generated.h"

class UEffectReceiverComponent;
class AProjectKapyCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStatusEffectDelegateSignature);


/**
 *
 */
UCLASS(Blueprintable)
class PROJECTKAPY_API AStatusEffect : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ElapsedTime;
	UPROPERTY(BlueprintReadOnly)
	float ElapsedTimeToProcess;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsCompletedByDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PowerModifier = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeToProcess;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UEffectReceiverComponent* ReceiverComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AProjectKapyCharacter* Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AttachToOwner;

	FStatusEffectDelegateSignature OnEffectCompleted;

public:
	AStatusEffect();

	virtual void Init(UEffectReceiverComponent* Receiver);

	virtual void InitEffect();

	UFUNCTION(BlueprintImplementableEvent)
	void InitEffect_BP();

	virtual void EndEffect();

	UFUNCTION(BlueprintImplementableEvent)
	void EndEffect_BP();

	virtual void ProcessEffect();

	UFUNCTION(BlueprintImplementableEvent)
	void ProcessEffect_BP();

	virtual void TickProcess(float dt);

	UFUNCTION(BlueprintImplementableEvent)
	void TickProcess_BP(float dt);

	virtual void Process();
	virtual bool IsCompleted();
	virtual void Clear();

};
