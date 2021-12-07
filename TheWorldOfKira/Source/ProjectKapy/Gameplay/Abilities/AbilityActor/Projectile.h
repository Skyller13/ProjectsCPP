// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectKapy/Gameplay/Abilities/AbilityActor/AbilityActorBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ProjectKapy/EnumCharacterType.h"

#include "Projectile.generated.h"

class AStatusEffect;
class UProjectileBaseAsset;

USTRUCT(BlueprintType)
struct FProjectileProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DistanceMax = 10000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DeathTime = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageDirectHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanExplode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bCanExplode"))
	bool bCanHitShooter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bCanExplode"))
	float RadiusExplosion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bCanExplode"))
	float DamageExplosion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bCanExplode"))
	bool bDamageExplosionIsScaled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LaunchSpeed = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseGravity = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bUseGravity"))
	float GravityOverride = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<AStatusEffect>> EffectsToApply;
};

UCLASS()
class PROJECTKAPY_API AProjectile : public AAbilityActorBase
{
	GENERATED_BODY()

protected:

	float ElapsedTimeSinceSpawn = 0.f;

public:
	// Sets default values for this actor's properties
	AProjectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovement = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* SphereCollision = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FProjectileProperties Properties;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectQueryFilter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AProjectile> ReflectProjectile;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	float DistanceTraveled;

	bool bMustBeDestroyed;
	
	bool bHaveExplosed;

	ECharacterType ShooterType;

	UPROPERTY()
	TArray<AActor*> ActorsHit;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	virtual void Init(UAbilityBase* Ability) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnAbilityActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                        const FHitResult& SweepResult) override;

	virtual void OnAbilityActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	                               UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	                               const FHitResult& Hit) override;

	UFUNCTION(BlueprintCallable)
	void SetupProjectile(FVector Direction);

	UFUNCTION(BlueprintCallable)
	virtual void OverrideProperties(FProjectileProperties NewProperties);

	void ApplyProjectileDamageToActor(AActor* ActorHit, float Damage, ECharacterType _type);

	UFUNCTION(BlueprintCallable)
	void ExplodeProjectile(AActor* ActorHit = nullptr);

	void OnProjectileHit(AActor* ActorHit);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnProjectileHit(AActor* ActorHit);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnProjectileExplode(AActor* ActorHit);
};
