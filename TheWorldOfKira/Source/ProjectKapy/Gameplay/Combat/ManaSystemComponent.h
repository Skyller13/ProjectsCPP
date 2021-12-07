// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ManaSystemComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTKAPY_API UManaSystemComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana", meta = (ClampMin = "0.0", UIMin = "1.0"))
	float ManaBase;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Mana")
	float ManaCurrent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana")
	float ManaRegerationRate;

public:
	// Sets default values for this component's properties
	UManaSystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Mana")
    virtual void AddMana(float _mana);
	
	UFUNCTION(BlueprintCallable, Category = "Mana")
	virtual void ConsumeMana(float _mana);

	UFUNCTION(BlueprintCallable, Category = "Mana")
	virtual float GetManaPercent();

	UFUNCTION(BlueprintCallable, Category = "Mana")
	virtual bool HaveEnoughtMana(float _manaCost);
};
