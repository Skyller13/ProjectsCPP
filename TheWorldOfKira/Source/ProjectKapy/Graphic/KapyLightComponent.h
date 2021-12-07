// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KapyLightComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTKAPY_API UKapyLightComponent : public UActorComponent
{
	GENERATED_BODY()

	protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DayNightManagerSettings")
	class ADayNightCycleManager* DayNightCycleManager;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNightManagerSettings")
	bool ShouldBeOnByDay = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNightManagerSettings")
	bool ShouldBeOnByNight = true;

public:	
	// Sets default values for this component's properties
	UKapyLightComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintSetter)
	void SetShouldBeOnByDay(bool _b) { ShouldBeOnByDay = _b; }

	UFUNCTION(BlueprintSetter)
	void SetShouldBeOnByNight(bool _b) { ShouldBeOnByNight = _b; }

	UFUNCTION(BlueprintSetter)
    bool GetShouldBeOnByDay() { return ShouldBeOnByDay; }

	UFUNCTION(BlueprintSetter)
    bool GetShouldBeOnByNight() { return ShouldBeOnByNight; }
};
