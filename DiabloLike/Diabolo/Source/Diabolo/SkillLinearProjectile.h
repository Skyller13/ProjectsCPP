// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillActor.h"
#include "SkillLinearProjectile.generated.h"

/**
 * 
 */
UCLASS()
class DIABOLO_API ASkillLinearProjectile : public ASkillActor
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	ASkillLinearProjectile();

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float distanceTravelled = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxRange = 1500.0f;

	FVector lastFramePosition;

	virtual void Init(USkillBase* _skill, FVector _direction, float _speed, float _maxRange);

	void TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	void TriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};
