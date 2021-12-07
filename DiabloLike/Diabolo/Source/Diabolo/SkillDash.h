// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "SkillDash.generated.h"

/**
 * 
 */
UCLASS()
class DIABOLO_API USkillDash : public USkillBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float dashExecute = 0.1f;

	UPROPERTY(EditAnywhere)
	float dashStop = 0.1f;

public:
	virtual void Execute() override;

	void Dash();

	void StopDashing();
};
