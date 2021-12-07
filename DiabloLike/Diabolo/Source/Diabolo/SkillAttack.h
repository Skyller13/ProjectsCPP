// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "SkillAttack.generated.h"

/**
 * 
 */
UCLASS()
class DIABOLO_API USkillAttack : public USkillBase
{
	GENERATED_BODY()

protected:
	virtual void Execute() override;
	
public:

	virtual void SkillBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION(BlueprintCallable)
	void SpawnDetectionMesh();

	UFUNCTION(BlueprintCallable)
	void SpawnDetectionMeshSpecificTransform(FTransform _transform);
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASkillCollisionDetection> detectionMesh;
};
