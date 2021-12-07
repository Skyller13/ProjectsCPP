// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectKapy/Gameplay/Abilities/AbilityBase.h"
#include "LaserAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKAPY_API ULaserAbility : public UAbilityBase
{
	GENERATED_BODY()


public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<USceneComponent*> LaserArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentLaserLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LaserLengthMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BlendFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsBlending;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BlendTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ElapsedTime;


public:
	ULaserAbility();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetBlend(float Time);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnBlendEnded();
};
