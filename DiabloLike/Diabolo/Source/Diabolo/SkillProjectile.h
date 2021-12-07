// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "SkillProjectile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamageTaken);

UCLASS()
class DIABOLO_API USkillProjectile : public USkillBase
{
	GENERATED_BODY()

protected:

    virtual void Execute() override;
    
public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FOnDamageTaken OnDamageTaken;

    virtual void TickSkill(float _DeltaTime) override;

    virtual void SkillBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float projectileSpeed = 2500.0f;

    UPROPERTY(EditAnywhere)
    TSubclassOf<class ASkillLinearProjectile> ProjectileBP;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    ASkillLinearProjectile* projectileRef;

    UFUNCTION(BlueprintCallable)
    void SpawnBullet(FTransform _transform);
};
