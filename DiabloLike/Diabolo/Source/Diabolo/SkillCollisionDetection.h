// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillActor.h"
#include "SkillCollisionDetection.generated.h"

/**
 * 
 */
UCLASS()
class DIABOLO_API ASkillCollisionDetection : public ASkillActor
{
	GENERATED_BODY()

protected:

	ASkillCollisionDetection();

	void BeginPlay() override;

public:

	virtual void Tick(float DeltaSeconds) override;

	void TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	void TriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};
