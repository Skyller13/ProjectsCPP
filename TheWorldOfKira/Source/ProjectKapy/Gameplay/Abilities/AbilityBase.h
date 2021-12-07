// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbilityManagerComponent.h"
#include "UObject/Object.h"
#include "AbilityBase.generated.h"

class AProjectKapyCharacter;
class AAbilityActorBase;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAbilityEventSignature);

UCLASS(BlueprintType, Blueprintable, Abstract)
class PROJECTKAPY_API UAbilityBase : public UObject
{
	GENERATED_BODY()

public:
	UAbilityBase();

protected:

	UPROPERTY()
	AProjectKapyCharacter* Character;

	UPROPERTY()
	UAbilityManagerComponent* AbilityManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsExecuting;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	float ExecutingElapsedTime;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Input)
	bool bIsPress;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Input)
	float PressElapsedTime;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Input)
	FName ActionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSensitiveDamage = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<EDamageType>> TypeDamageSensitive;

	UPROPERTY(BlueprintAssignable)
	FAbilityEventSignature OnExecute;

	UPROPERTY(BlueprintAssignable)
	FAbilityEventSignature OnInterrupt;


public:
	virtual void Init(UAbilityManagerComponent* AbilityMng);
	virtual void Destroy();
	virtual void Tick(float DeltaTime);
	virtual void AbilityPressed();
	virtual void AbilityRelease();
	virtual void Interrupt();
	UFUNCTION(BlueprintCallable)
	virtual void Execute();
	virtual void EndExecute();
	virtual void OnCharacterReceiveDamage(AActor* DamagedActor, float Value, AActor* DamageCauser);


	UFUNCTION(BlueprintImplementableEvent)
	void BP_Init();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_Tick(float DeltaTime);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_Interrupt();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_Execute();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_EndExecute();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnDestroy();

	//Return if AbilityManager can call Execute
	virtual bool IsExecutionValid() const;

	//Collisions
	UFUNCTION()
	virtual void OnAbilityBeginOverlap(AAbilityActorBase* AbilityActor, UPrimitiveComponent* OverlappedComponent,
	                                   AActor* OtherActor,
	                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                   const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnAbilityEndOverlap(AAbilityActorBase* AbilityActor, UPrimitiveComponent* OverlappedComponent,
	                                 AActor* OtherActor,
	                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UFUNCTION()
	virtual void OnAbilityHit(AAbilityActorBase* AbilityActor, UPrimitiveComponent* HitComponent, AActor* OtherActor,
	                          UPrimitiveComponent* OtherComp,
	                          FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnAbilityBeginOverlap(AAbilityActorBase* AbilityActor, UPrimitiveComponent* OverlappedComponent,
	                              AActor* OtherActor,
	                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                              const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnAbilityEndOverlap(AAbilityActorBase* AbilityActor, UPrimitiveComponent* OverlappedComponent,
	                            AActor* OtherActor,
	                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnAbilityHit(AAbilityActorBase* AbilityActor, UPrimitiveComponent* HitComponent, AActor* OtherActor,
	                     UPrimitiveComponent* OtherComp,
	                     FVector NormalImpulse, const FHitResult& Hit);

	//Helper

	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "AbilityActor"))
	virtual AAbilityActorBase* InstantiateAbilityActor(TSubclassOf<AAbilityActorBase> AbilityActor);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AProjectKapyCharacter* GetCharacter() const { return Character; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UAbilityManagerComponent* GetAbilitiesManager() const { return AbilityManager; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsPress() const { return bIsPress; }

	virtual bool IsExecuting() const;

	UFUNCTION(BlueprintCallable)
	virtual UWorld* GetWorld() const override;

UFUNCTION(BlueprintCallable)
	virtual bool IsDamageTypeSensitive(EDamageType _damageType);
	
};
