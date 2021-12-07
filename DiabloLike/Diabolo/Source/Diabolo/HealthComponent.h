// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIABOLO_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealthMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentPercentage;;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BufferHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float timeRemain;

	UPROPERTY(EditAnywhere, Category = "Event")
	FOnDamageReceive OnDamageReceive;

	float tempBuffer;

	//Animations
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	class UAnimationAsset* CrouchDeathAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	TArray<class UAnimationAsset*> StandDeathAnimationList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	class UAnimMontage* HitAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	class UAnimMontage* HealAnimation;
	
	//UI
	class UUserWidget* HealthUI;

	//Methods
	UFUNCTION(BlueprintCallable)
    void CalculatePercentage();

	UFUNCTION(BlueprintCallable)
    void AddHealth(float _value);

	UFUNCTION(BlueprintCallable)
	void RemoveHealth(float _value);

	UFUNCTION(BlueprintCallable)
	bool IsOwnerAlive() const;
};
