// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectKapy/Gameplay/Combat/LifeSystemComponent.h"

#include "AbilityManagerComponent.generated.h"

class UAbilityBase;
class UAttackAbility;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityDelegateSignature, UAbilityBase*, Ability);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTKAPY_API UAbilityManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAbilityManagerComponent();

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TArray<UAbilityBase*> AbilitiesInstanced;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<TSubclassOf<UAbilityBase>> Abilities;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	bool bBlockRequest = false;

public:

	UPROPERTY(BlueprintAssignable)
	FAbilityDelegateSignature OnAbilityRequestExecute;

	UPROPERTY(BlueprintAssignable)
	FAbilityDelegateSignature OnAbilityAdded;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	template <class T>
	T* GetAbilityByClass() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, const UAbilityBase>::Value,
			"'T' template parameter to GetAbilityByClass must be derived from UAbilityBase");
		return (T*)GetAbilityByClass(T::StaticClass());
	}

	UFUNCTION(BlueprintCallable)
	virtual void RequestAbilityPress(FName ActionName);
	UFUNCTION(BlueprintCallable)
	virtual void RequestAbilityRelease(FName ActionName);
	UFUNCTION(BlueprintCallable)
	virtual bool RequestAbilityExecute(FName ActionName);
	UFUNCTION(BlueprintCallable)
	virtual bool RequestAbilityEndExecute(FName ActionName);
	UFUNCTION(BlueprintCallable)
	virtual void RequestAbilityInterrupt(FName ActionName);

	virtual void DestroyComponent(bool bPromoteChildren) override;

	UFUNCTION(BlueprintCallable)
	virtual void InterruptAll();

	UFUNCTION(BlueprintCallable, meta = (ComponentClass = "UAbilityBase"), meta = (DeterminesOutputType = "NewAbility"))
	virtual UAbilityBase* AddAbility(UAbilityBase* NewAbility);

	UFUNCTION(BlueprintCallable, meta = (ComponentClass = "AbilityBase"), meta = (DeterminesOutputType = "AbilityToFind"
	))
	virtual UAbilityBase* GetAbilityByClass(TSubclassOf<UAbilityBase> AbilityToFind) const;
	UFUNCTION(BlueprintCallable)
	virtual UAbilityBase* GetAbilityByName(FName AbilityName) const;

	UFUNCTION(BlueprintCallable)
	virtual UAbilityBase* GetAbilityExecuting() const;

	UFUNCTION(BlueprintCallable)
	virtual bool IsAbilityExecuting() const;

	UFUNCTION(BlueprintCallable)
	virtual void SetBlockRequest(bool Value);

	UFUNCTION(BlueprintCallable)
	virtual void OnDamage(AActor* DamagedActor, float Value, AActor* DamageCauser, EDamageType TypeDamage);
};
