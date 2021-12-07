// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectKapy/EnumCharacterType.h"
#include "LifeSystemComponent.generated.h"


class AProjectKapyCharacter;


UENUM(BlueprintType, Blueprintable)
enum EDamageType
{
	DT_None = 0 UMETA(DisplayName = "None"),
    DT_Melee UMETA(DisplayName = "Melee"),
    DT_Distance UMETA(DisplayName = "Distance"),
    DT_Dot UMETA(DisplayName = "Dot")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLifeDelegateSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FLifeDelegateDamageSignature, AActor*, DamagedActor, float, Value,
                                               AActor*, DamageCauser, EDamageType, DamageType);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLifeDelegateFloatSignature, float, Value);

UCLASS(ClassGroup = (Custom), Blueprintable, meta = (BlueprintSpawnableComponent))
class PROJECTKAPY_API ULifeSystemComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	bool bAlreadyDead;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<ECharacterType>> SensitiveType;

	UPROPERTY
	(EditAnywhere, BlueprintReadWrite)
	float Armor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1.0", UIMin = "1.0"))
	float HpBase;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	float HpCurrent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanDie;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MeleeDamageModifier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DistanceDamageModifier = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanReceiveDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanReceiveHeal;

	UPROPERTY()
	ACharacter* Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* RightHitAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* LeftHitAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* FrontHitAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* BackHitAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeWithoutHitForRegen = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ElaspedTimeWithoutHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LifeRegenerationRate = 0.f;

	// Sets default values for this component's properties
	ULifeSystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void ApplyHealToHP(float _heal);

	UPROPERTY(BlueprintAssignable)
	FLifeDelegateSignature OnDeath;

	UPROPERTY(BlueprintAssignable)
	FLifeDelegateSignature OnRevive;

	UPROPERTY(BlueprintAssignable)
	FLifeDelegateSignature OnDeathEndAnim;

	UPROPERTY(BlueprintAssignable)
	FLifeDelegateDamageSignature OnDamageReceived;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FLifeDelegateFloatSignature OnHealReceived;

	UFUNCTION(BlueprintCallable, Category=Life)
	float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable, Category=Life)
	void EndDeathAnimBroadcast();

	UFUNCTION(BlueprintCallable, Category=Life)
	void Revive();

	UFUNCTION(BlueprintCallable, Category=Life)
	void Death();

	UFUNCTION(BlueprintCallable, Category=Damage)
	void ApplyDamageToHP(float _Damage, AActor* Causer, ECharacterType TypeCauser, EDamageType TypeDamage);

	UFUNCTION(BlueprintCallable, Category=Damage)
	void ApplyDamageAtLocalPos(float _Damage, FVector _LocalPos, AActor* Causer, ECharacterType TypeCauser, EDamageType TypeDamage);

	UFUNCTION(BlueprintCallable, Category=Damage)
	void ApplyDamageAtPos(float _Damage, FVector _WorldPos, AActor* Causer, ECharacterType TypeCauser, EDamageType TypeDamage = DT_None);

	void PlayAnimHitFromAngle(float _AngleDeg);

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable)
	void SetHPMax(float HPMax);

	UFUNCTION(BlueprintCallable)
	bool IsDamageSensible(ECharacterType _type);
};
