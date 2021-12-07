// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectKapy/Gameplay/Abilities/AbilityBase.h"

#include "ShootAbilityBase.generated.h"

class UProjectileBaseAsset;
class AProjectile;
class UManaSystemComponent;
class ACastFX;
class UProjectileTemplate;


UENUM(BlueprintType, Blueprintable)
enum FiringState
{
	FS_None = 0 UMETA(DisplayName = "None"),
	FS_Cast UMETA(DisplayName = "Cast"),
	FS_Firing UMETA(DisplayName = "Firing"),
	FS_Recovery UMETA(DisplayName = "Recovery")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShootAbilityDelegateSignature, UProjectileTemplate*, Projectile);

/**
 * 
 */
UCLASS()
class PROJECTKAPY_API UShootAbilityBase : public UAbilityBase
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UProjectileTemplate> ProjectileTemplateSubclass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	UProjectileTemplate* ProjectileTemplate;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	TEnumAsByte<FiringState> CurrentState;

	float RecoveryTimer;
	float CastTimer;

	float TimeToLaunchProjectile; //Anim notif
	float TimerToLaunchProjectile; //Anim notif

	//NB Shoot Done in Firing State
	int32 ShootCount;
	float ShootTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector AimDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Socket;

	UPROPERTY()
	UManaSystemComponent* ManaSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACastFX> CastFX;

	UPROPERTY()
	ACastFX* CastFXInstanced;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDamageSensitive;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FName SectionNameRepeat = "Repeat";

public:
	UPROPERTY(BlueprintAssignable)
	FShootAbilityDelegateSignature OnShootBegin;

public:
	UShootAbilityBase();

public:
	virtual void Init(UAbilityManagerComponent* AbilityMng) override;
	virtual void Destroy() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Interrupt() override;
	virtual void Execute() override;
	virtual bool IsExecutionValid() const override;
	virtual void AbilityPressed() override;
	virtual void OnCharacterReceiveDamage(AActor* DamagedActor, float Value, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	virtual bool CanShoot();

	UFUNCTION(BlueprintCallable)
	virtual void SetAimDirection(FVector Direction);
	virtual void LaunchProjectile(UProjectileTemplate* Projectile);
	virtual void PlayFiringAnim(UProjectileTemplate* Projectile);
	virtual AProjectile* InstanciateProjectile(UProjectileTemplate* Projectile, FTransform Transform);

	UFUNCTION(BlueprintCallable)
	void AimAt(FVector TargetLocation);
	UFUNCTION(BlueprintCallable)
	virtual bool CanFireProjectile(UProjectileTemplate* Projectile);
	UFUNCTION(BlueprintCallable)
	virtual float GetChargePercent() const;
	UFUNCTION(BlueprintCallable)
	float GetTimeBeforeShootPercent() const;

	UFUNCTION(BlueprintCallable)
	UProjectileTemplate* GetMainProjectileTemplate() const;

	UFUNCTION(BlueprintCallable)
	void SetMainProjectileTemplate(TSubclassOf<UProjectileTemplate> MainProjectileTemplate);
};
