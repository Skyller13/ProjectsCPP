// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectKapy/Gameplay/Abilities/AbilityBase.h"
#include "AspirationAbility.generated.h"


USTRUCT()
struct FAttractedActor
{
	GENERATED_BODY()

	UPROPERTY()
	AProjectKapyCharacter* CharRef;

	UPROPERTY()
	float GroundFriction;

	UPROPERTY()
	float BrakingDeceleration;
};

/**
 * 
 */
UCLASS()
class PROJECTKAPY_API UAspirationAbility : public UAbilityBase
{
	GENERATED_BODY()

protected:

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Strength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RadiusMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ZFactor = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<EObjectTypeQuery>> TargetObjectTypes;

	UPROPERTY()
	TArray<FAttractedActor> CharactersAffected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BlendFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsBlending;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BlendTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ElapsedTime;

public:
	UAspirationAbility();

protected:

	void ClearActorAffected();

	virtual bool ContainsCharacter(AProjectKapyCharacter* Char);
	virtual void AddCharacter(AProjectKapyCharacter* Char);
	virtual void RemoveCharacter(FAttractedActor& Char);
public:
	virtual void Tick(float DeltaTime) override;
	virtual void Interrupt() override;
	virtual bool IsExecutionValid() const override;
	UFUNCTION(BlueprintCallable)
	void SetBlend(float Time);
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnBlendEnded();
};
