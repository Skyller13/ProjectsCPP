// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillAOE.h"
#include "SkillGrenade.generated.h"

/**
 * 
 */
UCLASS()
class DIABOLO_API USkillGrenade : public USkillAOE
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Timer")
	float elapsedTime = 0.f;

public:

	UPROPERTY(EditAnywhere, Category = "Timer")
	float timerUntilExplosion = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isGrenadeThrew = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> grenadeObj;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* grenadeRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ImpulseDirection;

protected:
	virtual void Execute() override;

	virtual void TickSkill(float _DeltaTime) override;

	virtual void ExplosionGrenade();

public:

	UFUNCTION(BlueprintImplementableEvent)
    void Event_ThrowGrenade();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Event_SetGrenadePhysics();

	UFUNCTION(BlueprintImplementableEvent)
	void Event_OnExplosionEvent();
	
	virtual void ThrowGrenade();

	virtual void DestroyGrenade();
	
	virtual void SkillBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
