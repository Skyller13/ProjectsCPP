// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "SkillHeal.generated.h"

/**
 * 
 */
UCLASS()
class DIABOLO_API USkillHeal : public USkillBase
{
	GENERATED_BODY()

protected:

    virtual void Execute() override;

    virtual void TickSkill(float _DeltaTime) override;

private:

    float healTickTime = 1.0f;

    float currentHealTimer = 0.0f;

    int count = 0;
    
public:

    UPROPERTY(EditAnywhere)
    bool isOnTime = false;

    UPROPERTY(EditAnywhere)
    float healingTime = 1.0f;

    UPROPERTY(EditAnywhere)
    int nbHealTick = 3;

    UPROPERTY(EditAnywhere)
    int healValue = 15;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
    TSubclassOf<AActor> healParticles;

    UPROPERTY()
    AActor* healActorRef;

    UFUNCTION()
    void CreateHeal();
};
