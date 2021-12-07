// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "GameFramework/Actor.h"
#include "DayNightCycleManager.generated.h"

UENUM(Blueprintable)
enum TYPEDayNightMoment
{
	MORNING		UMETA(DisplayName = "Morning"),
	DAY			UMETA(DisplayName = "Day"),
	EVENING		UMETA(DisplayName = "Evening"),
	NIGHT		UMETA(DisplayName = "Night"),
	NUM			UMETA(DisplayName = "Num"),
};

USTRUCT(BlueprintType)
struct FDirectionalLightVariables
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditInstanceOnly)
	class ADirectionalLight* reference;

	UPROPERTY(EditInstanceOnly)
	class UDirectionalLightComponent* component;

	UPROPERTY(EditInstanceOnly)
	float initIntensity;
};

USTRUCT(BlueprintType)
struct FPointSpotLightVariables
{
	GENERATED_USTRUCT_BODY()

    UPROPERTY(EditInstanceOnly)
	class ALight* reference;

	UPROPERTY(EditInstanceOnly)
	class UKapyLightComponent* component;

	UPROPERTY(EditInstanceOnly)
	float initIntensity;
};



UCLASS()
class PROJECTKAPY_API ADayNightCycleManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADayNightCycleManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//References
		//Directional Light
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDirectionalLightVariables DirectionalLight;

		//Point & Spot Lights
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (TitleProperty = "NamePrivate"))
	TArray<FPointSpotLightVariables> PointSpotLights;

	UFUNCTION(BlueprintCallable)
	void PointSpotLightRegister(AActor* _ref, UKapyLightComponent* _comp);

	UFUNCTION(BlueprintCallable)
    void PointSpotLightRemove(ALight* _ref);
    

		//Fog
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AExponentialHeightFog* HeightFog;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UExponentialHeightFogComponent* HeightFogComponent;


	//Variables

		//Switches
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight Cycle settings")
	bool IsCycleRunning = true;

		//General
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight Cycle settings")
	float DayCycleLength = 1800.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DayNight Cycle settings")
	float DayCycleTime = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DayNight Cycle settings")
	float CurrentMomentTime = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DayNight Cycle settings")
	float CurrentMomentTimeNormalized = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight Cycle settings", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float DayProportion = 0.7f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight Cycle settings", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MorningEveningProportion = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight Cycle settings")
	TEnumAsByte<TYPEDayNightMoment> DayNightMoment;
	TEnumAsByte<TYPEDayNightMoment> DayNightMomentOld;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight Cycle settings")
	TEnumAsByte<TYPEDayNightMoment> InitialDayNightMoment;

		//Sun angle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight Cycle settings | Sun angle", meta = (ClampMin = "-180.0", ClampMax = "180.0", UIMin = "-180.0", UIMax = "180.0"))
	float MorningLightAngle = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight Cycle settings | Sun angle", meta = (ClampMin = "-180.0", ClampMax = "180.0", UIMin = "-180.0", UIMax = "180.0"))
	float DayLightAngleBegin = -60.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight Cycle settings | Sun angle", meta = (ClampMin = "-180.0", ClampMax = "180.0", UIMin = "-180.0", UIMax = "180.0"))
	float DayLightAngleEnd = -110.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight Cycle settings | Sun angle", meta = (ClampMin = "-180.0", ClampMax = "180.0", UIMin = "-180.0", UIMax = "180.0"))
	float EveningLightAngle = -180.f;

		//Sun Color
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight Cycle settings | Sun Color")
	FLinearColor MorningSunColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight Cycle settings | Sun Color")
	FLinearColor DaySunColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight Cycle settings | Sun Color")
	FLinearColor EveningSunColor = FLinearColor::White;


		//Fog Color
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight Cycle settings | Fog Color")
	FLinearColor MorningFogColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight Cycle settings | Fog Color")
	FLinearColor DayFogColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight Cycle settings | Fog Color")
	FLinearColor EveningFogColor = FLinearColor::White;

	//Sky Color
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight Cycle settings | Sky Color")
	bool ShouldChangeSkyColor = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight Cycle settings | Sky Color")
	FLinearColor DaySkyColor = FLinearColor::White;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight Cycle settings | Sky Color")
	FLinearColor NightSkyColor = FLinearColor::White;

	

	//Get/Set Function
	UFUNCTION(BlueprintGetter)
	float GetDayCycleTime();

	UFUNCTION(BlueprintGetter)
    float GetDayCycleTimeNormalized();

	UFUNCTION(BlueprintGetter)
    float GetDayNightMomentTimer() const {return CurrentMomentTime;}
	
	UFUNCTION(BlueprintGetter)
	float GetDayNightMomentTimerNormalized() const {return CurrentMomentTimeNormalized;}

	UFUNCTION(BlueprintGetter)
	TEnumAsByte<TYPEDayNightMoment> GetDayNightMoment() const {return DayNightMoment;}

	protected:
	//System Functions
	UFUNCTION()
	void Init();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DayIncrementation(float DeltaTime);
	virtual void DayIncrementation_Implementation(float DeltaTime);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ONMomentChange(TYPEDayNightMoment _newMoment);
	virtual void ONMomentChange_Implementation(TYPEDayNightMoment _newMoment);

	

	//-----MomentTransition--------------//
	UFUNCTION()
    void MomentTransition();
	

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MorningTransition();
	virtual void MorningTransition_Implementation();
	UPROPERTY(EditDefaultsOnly, Category = "DayNight Cycle curves | Morning")
	UCurveFloat* MorningCurveFadeDirectional = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "DayNight Cycle curves | Morning")
	UCurveFloat* MorningCurveFogDensity = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "DayNight Cycle curves | Morning")
	UCurveFloat* MorningCurveFogHeight = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "DayNight Cycle curves | Morning")
    UCurveFloat* MorningFogStartDistance = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "DayNight Cycle curves | Morning")
	UCurveFloat* MorningCurveShade = nullptr;
	
	

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void DayTransition();
    virtual void DayTransition_Implementation();
	UPROPERTY(EditDefaultsOnly, Category = "DayNight Cycle curves | Day")
	UCurveFloat* DayCurveFogDensity = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "DayNight Cycle curves | Day")
	UCurveFloat* DayCurveFogHeight = nullptr;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void EveningTransition();
    virtual void EveningTransition_Implementation();
	UPROPERTY(EditDefaultsOnly, Category = "DayNight Cycle curves | Evening")
	UCurveFloat* EveningCurveFadeDirectional = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "DayNight Cycle curves | Evening")
	UCurveFloat* EveningCurveFogDensity = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "DayNight Cycle curves | Evening")
	UCurveFloat* EveningCurveFogHeight;
	UPROPERTY(EditDefaultsOnly, Category = "DayNight Cycle curves | Evening")
	UCurveFloat* EveningFogStartDistance = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "DayNight Cycle curves | Evening")
	UCurveFloat* EveningCurveShade = nullptr;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void NightTransition();
    virtual void NightTransition_Implementation();
	UPROPERTY(EditDefaultsOnly, Category = "DayNight Cycle curves | Night")
	UCurveFloat* NightCurveFogDensity = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "DayNight Cycle curves | Night")
	UCurveFloat* NightCurveFogHeight;
	//END--MomentTransition--------------//

	//-----Debug Commande Functions------//
	UFUNCTION(BlueprintCallable)
	void DebugPauseDayProgression() { IsCycleRunning = false;}

	UFUNCTION(BlueprintCallable)
    void DebugResumeDayProgression() { IsCycleRunning = true;}

	UFUNCTION(BlueprintCallable)
    void DebugSetMorning() { DayCycleTime = 0.f;}

	UFUNCTION(BlueprintCallable)
    void DebugSetDay() { DayCycleTime = (DayCycleLength *  MorningEveningProportion);}

	UFUNCTION(BlueprintCallable)
    void DebugSetEvening() { DayCycleTime = (DayCycleLength *  DayProportion) - (DayCycleLength *  MorningEveningProportion);}

	UFUNCTION(BlueprintCallable)
    void DebugSetNight() { DayCycleTime = (DayCycleLength *  DayProportion);}

		//Fast forward
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight Cycle settings | Fast Forward")
	bool IsInFastForward = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight Cycle settings | Fast Forward")
	TEnumAsByte<TYPEDayNightMoment> FastForwardTargetMoment;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DayNight Cycle settings | Fast Forward")
	float FastForwardTimer = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight Cycle settings | Fast Forward")
	float FastForwardTimerMax = 5.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DayNight Cycle settings | Fast Forward")
	float FastForwardDayInitTimeValue = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DayNight Cycle settings | Fast Forward")
	float FastForwardDayTargetTimeValue = 0.f;

	UFUNCTION(BlueprintCallable)
    void FastForwardTime(TYPEDayNightMoment _targetMoment, float _transitionDuration = 5.f);

	UFUNCTION()
    void FastForwardTransition(float DeltaTime);
	
	//END--Debug Commande Functions------//
	
};
