// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LifeSystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageReceive, int32, damageValue);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANTIVIRUSTACTICAL_API ULifeSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULifeSystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	float timerDeath = 0.f;

	float maxTimerDeath = 2.0f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HealthMax;

	UPROPERTY(EditAnywhere, BlueprintAssignable, BlueprintReadWrite, Category = "Event")
	FOnDamageReceive OnDamageReceive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	class UAnimMontage* HitAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	class UAnimMontage* HealAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	class UAnimMontage* DeathAnimation;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere)
	bool isDead = false;

	//Methods
	UFUNCTION(BlueprintCallable)
    void AddHealth(int32 _value);

	UFUNCTION(BlueprintCallable)
    void RemoveHealth(int32 _value);

	UFUNCTION(BlueprintCallable)
    bool IsOwnerAlive() const;

	UFUNCTION()
	void OnOwnerDeath();
};