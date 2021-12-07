// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class DIABOLO_API USkillBase : public UObject
{
	GENERATED_BODY()

protected:

	virtual void Execute();
	
	virtual void EndExecution();

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkillManagerComponent* SkillManagerComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float range = 125;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float cooldown = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float castTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int damage = 0.0f;

	bool isExecuting = false;

	float currentCastTime = -1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float cooldownTimer = 0.0f;
	
	virtual bool RequestExecution();

	virtual void TickSkill(float _DeltaTime);

	virtual void SkillBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	virtual void SkillEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent)
	void Event_Execute();

	UFUNCTION(BlueprintImplementableEvent)
    void Event_EndExecution();

	UFUNCTION(BlueprintImplementableEvent)
    void Event_TickSkill(float _deltaTime);

	UFUNCTION(BlueprintImplementableEvent)
    void Event_SkillBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void Event_SkillEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
