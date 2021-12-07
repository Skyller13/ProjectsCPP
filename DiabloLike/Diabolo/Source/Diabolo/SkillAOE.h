// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillAttack.h"
#include "SkillAOE.generated.h"

/**
 * 
 */
UCLASS()
class DIABOLO_API USkillAOE : public USkillAttack
{
	GENERATED_BODY()

protected:
	virtual void Execute() override;

	virtual void ExecuteHandle();

public:
	virtual void SkillBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
