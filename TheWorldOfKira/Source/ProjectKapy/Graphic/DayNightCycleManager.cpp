// Fill out your copyright notice in the Description page of Project Settings.


#include "DayNightCycleManager.h"

#include <string>


#include "ProjectKapy/ProjectKapyCharacter.h"
#include "KapyLightComponent.h"
#include "Atmosphere/AtmosphericFog.h"
#include "Atmosphere/AtmosphericFogComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Engine/DirectionalLight.h"
#include "Engine/ExponentialHeightFog.h"
#include "Engine/PointLight.h"
#include "Engine/SpotLight.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ADayNightCycleManager::ADayNightCycleManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADayNightCycleManager::BeginPlay()
{
	Super::BeginPlay();

	Init();
}

// Called every frame
void ADayNightCycleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DayIncrementation(DeltaTime);

	MomentTransition();
	
}

void ADayNightCycleManager::PointSpotLightRegister(AActor* _ref, UKapyLightComponent* _comp)
{
	if (_ref)
	{
		FPointSpotLightVariables newLight;
		newLight.reference = Cast<ALight>(_ref);

		if (newLight.reference)
		{
			newLight.initIntensity = newLight.reference->GetBrightness();

			if (_comp)
			{
				newLight.component = _comp;
			}

			PointSpotLights.Add(newLight);
		}
		
	}
}

void ADayNightCycleManager::PointSpotLightRemove(ALight* _ref)
{
	const int num = PointSpotLights.Num();
	for(int i = 0; i < num; i++)
	{
		if (PointSpotLights[i].reference == _ref)
		{
			PointSpotLights.RemoveAt(i, 1, true);
			break;
		}
	}
}


float ADayNightCycleManager::GetDayCycleTime()
{
	return DayCycleTime;
}

float ADayNightCycleManager::GetDayCycleTimeNormalized()
{
	return (DayCycleTime / DayCycleLength);
}

void ADayNightCycleManager::Init()
{
	//Variables initialisation
	DayNightMoment = MORNING;
	DayNightMomentOld = DayNightMoment;

	//References setting

		//Directional light
	DirectionalLight.reference = Cast<ADirectionalLight>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ADirectionalLight::StaticClass()));

	if (DirectionalLight.reference)
	{
		DirectionalLight.initIntensity = DirectionalLight.reference->GetBrightness();

		DirectionalLight.reference->SetMobility(EComponentMobility::Movable);
		
		DirectionalLight.component = Cast<UDirectionalLightComponent>( DirectionalLight.reference->GetComponentByClass(UDirectionalLightComponent::StaticClass()));
		
	}

		//Atmospheric Fog
	HeightFog = Cast<AExponentialHeightFog>(UGameplayStatics::GetActorOfClass(GetWorld(), AExponentialHeightFog::StaticClass()));
	if (HeightFog)
	{
		HeightFogComponent = Cast<UExponentialHeightFogComponent>(HeightFog->GetComponentByClass(UExponentialHeightFogComponent::StaticClass()));
	}


	//Initial Moment setting
	switch (InitialDayNightMoment)
	{
	case MORNING:
		DayCycleTime = 0.f;
		DayNightMoment = InitialDayNightMoment;
		DayNightMomentOld = TYPEDayNightMoment::NUM;
		break;

	case DAY:
		DayCycleTime = (DayCycleLength *  MorningEveningProportion * DayProportion);
		DayNightMoment = InitialDayNightMoment;
		DayNightMomentOld = TYPEDayNightMoment::NUM;
		break;

	case EVENING:
		DayCycleTime = (DayCycleLength *  DayProportion) - (DayCycleLength *  MorningEveningProportion * DayProportion);
		DayNightMoment = InitialDayNightMoment;
		DayNightMomentOld = TYPEDayNightMoment::NUM;
		break;

	case NIGHT:
		DayCycleTime = (DayCycleLength *  DayProportion);
		DayNightMoment = InitialDayNightMoment;
		DayNightMomentOld = TYPEDayNightMoment::NUM;
		break;

	default:
		break;
	}
	
}


void ADayNightCycleManager::DayIncrementation_Implementation(float DeltaTime)
{
	if (DayCycleTime < DayCycleLength)
	{
		if (IsCycleRunning)
		{
			DayCycleTime += DeltaTime;
		}

		//Set the correct moment function of current time
		float morningMark = (DayProportion * DayCycleLength) * MorningEveningProportion;
		float dayMark = (DayProportion * DayCycleLength) - morningMark;
		float eveningMark = (DayProportion * DayCycleLength);
		float nightMark = DayCycleLength;


		if (DayCycleTime < morningMark)
			DayNightMoment = MORNING;
		else if (DayCycleTime < dayMark && DayCycleTime >= morningMark)
			DayNightMoment = DAY;
		else if (DayCycleTime < eveningMark && DayCycleTime >= dayMark)
			DayNightMoment = EVENING;
		else if (DayCycleTime < nightMark && DayCycleTime >= eveningMark)
			DayNightMoment = NIGHT;

		//Transition between moment
		if (DayNightMoment == DayNightMomentOld)
		{
			float timerMin = 0.f;
			float timerMax = 0.f;

			switch (DayNightMoment)
			{
			case MORNING:
				timerMin = 0.f;
				timerMax = (DayCycleLength *  MorningEveningProportion * DayProportion);
				break;

			case DAY:
				timerMin = (DayCycleLength *  MorningEveningProportion * DayProportion);
				timerMax = (DayCycleLength *  DayProportion) - (DayCycleLength *  MorningEveningProportion * DayProportion);
				break;

			case EVENING:
				timerMin = (DayCycleLength *  DayProportion) - (DayCycleLength *  MorningEveningProportion * DayProportion);
				timerMax = (DayCycleLength *  DayProportion);
				break;

			case NIGHT:
				timerMin = (DayCycleLength *  DayProportion);
				timerMax = DayCycleLength;
				break;

			default:
				break;
			}

			CurrentMomentTime = (DayCycleTime - timerMin);
			
			CurrentMomentTimeNormalized = UKismetMathLibrary::NormalizeToRange(DayCycleTime, timerMin, timerMax);
		}
		else
		{
			CurrentMomentTime = 0.f;
			CurrentMomentTimeNormalized = 0.f;

			DayNightMomentOld = DayNightMoment;

			ONMomentChange(DayNightMoment);
		}
	}
	else
	{
		//Morning event
		DayCycleTime = 0.f;
		DayNightMoment = MORNING;
	}

	FastForwardTransition(DeltaTime);
}

//Called when the moment is change to set the initial variables
void ADayNightCycleManager::ONMomentChange_Implementation(TYPEDayNightMoment _newMoment)
{
	if (DirectionalLight.reference && DirectionalLight.component)
	{
		//FRotator tempRot = DirectionalLight.reference->GetActorRotation();
		FRotator tempRot = FRotator(0.f, 0.f, 0.f);
		const int sizePointLights = PointSpotLights.Num();
		switch (_newMoment)
		{
		case MORNING:
			//Directional light angle
			tempRot.Pitch = MorningLightAngle;
			DirectionalLight.reference->SetActorRotation(tempRot, ETeleportType::None);

			//PointSpotLight
			
			for (int i = 0; i < sizePointLights; i++)
			{
				if (PointSpotLights[i].reference)
				{
					if (PointSpotLights[i].component)
					{
						//If the light should be on only by night
						if (!PointSpotLights[i].component->GetShouldBeOnByDay() && PointSpotLights[i].component->GetShouldBeOnByNight())
						{
							PointSpotLights[i].reference->SetBrightness(0.f);
						}
						//If the light should be on only by day
						else if (PointSpotLights[i].component->GetShouldBeOnByDay() && !PointSpotLights[i].component->GetShouldBeOnByNight())
						{
							PointSpotLights[i].reference->SetBrightness(PointSpotLights[i].initIntensity);
						}
					}
			
				}
			}
			

			//Shadow
			if (MorningCurveShade)
			{
				//Directional light Shadow fade
				DirectionalLight.component->SetShadowAmount(MorningCurveShade->GetFloatValue(0.f));
			}

			//Sun Color
			DirectionalLight.component->SetLightColor(MorningSunColor);

			//Fog Intensity
			if (MorningCurveFogDensity && HeightFogComponent)
			{
				HeightFogComponent->SetFogDensity(MorningCurveFogDensity->GetFloatValue(0.f));
				HeightFogComponent->SetFogHeightFalloff(MorningCurveFogHeight->GetFloatValue(0.f));

				HeightFogComponent->SetFogInscatteringColor(FLinearColor::LerpUsingHSV(MorningFogColor, DayFogColor, 0.f));
			}
			
			

			break;

		case DAY:
			//Directional light intensity
			if(DirectionalLight.reference)
				DirectionalLight.reference->SetBrightness(DirectionalLight.initIntensity);

			for (int i = 0; i < sizePointLights; i++)
			{
				if (PointSpotLights[i].reference)
				{
					if (PointSpotLights[i].component)
					{
						//If the light should be on only by night
						if (!PointSpotLights[i].component->GetShouldBeOnByDay() && PointSpotLights[i].component->GetShouldBeOnByNight())
						{
							PointSpotLights[i].reference->SetBrightness(0.f);
						}
						//If the light should be on only by day
						else if (PointSpotLights[i].component->GetShouldBeOnByDay() && !PointSpotLights[i].component->GetShouldBeOnByNight())
						{
							PointSpotLights[i].reference->SetBrightness(PointSpotLights[i].initIntensity);
						}
					}
				}
			}


			//Shadow
			if (MorningCurveShade)
			{
				//Directional light Shadow fade
				DirectionalLight.component->SetShadowAmount(MorningCurveShade->GetFloatValue(1.f));
			}

			//Sun Color
			DirectionalLight.component->SetLightColor(DaySunColor);

			//Fog Intensity
			if (MorningCurveFogDensity && HeightFogComponent)
			{
				HeightFogComponent->SetFogDensity(DayCurveFogDensity->GetFloatValue(0.f));
				HeightFogComponent->SetFogHeightFalloff(DayCurveFogHeight->GetFloatValue(0.f));

				HeightFogComponent->SetFogInscatteringColor(FLinearColor::LerpUsingHSV(MorningFogColor, DayFogColor, 1.f));
			}

			break;

		case EVENING:
			//Directional light intensity
			if(DirectionalLight.reference)
				DirectionalLight.reference->SetBrightness(DirectionalLight.initIntensity);

			for (int i = 0; i < sizePointLights; i++)
			{
				if (PointSpotLights[i].reference)
				{
					if (PointSpotLights[i].component)
					{
						//If the light should be on only by night
						if (PointSpotLights[i].component->GetShouldBeOnByDay() && !PointSpotLights[i].component->GetShouldBeOnByNight())
						{
							PointSpotLights[i].reference->SetBrightness(PointSpotLights[i].initIntensity);
						}
						//If the light should be on only by day
						else if (!PointSpotLights[i].component->GetShouldBeOnByDay() && PointSpotLights[i].component->GetShouldBeOnByNight())
						{
							PointSpotLights[i].reference->SetBrightness(0.f);
						}
					}
					
				}
			}

			//Sun Color
			DirectionalLight.component->SetLightColor(EveningSunColor);

			//Shadow
			if (EveningCurveShade)
			{
				//Directional light Shadow fade
				DirectionalLight.component->SetShadowAmount(EveningCurveShade->GetFloatValue(0.f));
			}

			//Fog
			if (EveningCurveFogDensity && HeightFogComponent)
			{
				HeightFogComponent->SetFogDensity(EveningCurveFogDensity->GetFloatValue(0.f));
				HeightFogComponent->SetFogHeightFalloff(EveningCurveFogHeight->GetFloatValue(0.f));

				HeightFogComponent->SetFogInscatteringColor(FLinearColor::LerpUsingHSV(DayFogColor, EveningFogColor, 0.f));
			}

			break;

		case NIGHT:
			//Directional light intensity
			if(DirectionalLight.reference)
				DirectionalLight.reference->SetBrightness(0.f);

			//Directional light angle
			tempRot.Pitch = 90.f;
			DirectionalLight.reference->SetActorRotation(tempRot, ETeleportType::None);

			for (int i = 0; i < sizePointLights; i++)
			{
				if (PointSpotLights[i].reference)
				{
					if (PointSpotLights[i].component)
					{
						//If the light should be on only by night
						if (PointSpotLights[i].component->GetShouldBeOnByDay() && !PointSpotLights[i].component->GetShouldBeOnByNight())
						{
							PointSpotLights[i].reference->SetBrightness(0.f);
						}
						//If the light should be on only by day
						else if (!PointSpotLights[i].component->GetShouldBeOnByDay() && PointSpotLights[i].component->GetShouldBeOnByNight())
						{
							PointSpotLights[i].reference->SetBrightness(PointSpotLights[i].initIntensity);
						}
					}
					
				}
			}

			//Shadow
			if (EveningCurveShade)
			{
				//Directional light Shadow fade
				DirectionalLight.component->SetShadowAmount(EveningCurveShade->GetFloatValue(1.f));
			}

			//Fog
			if (EveningCurveFogDensity && HeightFogComponent)
			{
				HeightFogComponent->SetFogDensity(EveningCurveFogDensity->GetFloatValue(1.f));
				HeightFogComponent->SetFogHeightFalloff(EveningCurveFogHeight->GetFloatValue(1.f));

				HeightFogComponent->SetFogInscatteringColor(FLinearColor::LerpUsingHSV(DayFogColor, EveningFogColor, 1.f));
			}

			
			break;

		default:
			break;
		}
	}
	
}

void ADayNightCycleManager::MomentTransition()
{
	switch (DayNightMoment)
	{
	case MORNING:
		MorningTransition();
		break;

	case DAY:
		DayTransition();
		break;

	case EVENING:
		EveningTransition();
		break;

	case NIGHT:
		NightTransition();
		break;

	default:
		break;
	}
}

void ADayNightCycleManager::MorningTransition_Implementation()
{
	//DIRECTIONAL PARAMETERS TRANSITION////////////////
	if (MorningCurveFadeDirectional)
	{
		if(DirectionalLight.reference)
		{
			//Directional light intensity
			DirectionalLight.reference->SetBrightness(DirectionalLight.initIntensity * MorningCurveFadeDirectional->GetFloatValue(CurrentMomentTimeNormalized));

			//Directional light angle
			FRotator tempRot = FRotator(0.f, 0.f, 0.f);
			tempRot.Pitch = FMath::Lerp(MorningLightAngle, DayLightAngleBegin, CurrentMomentTimeNormalized);
			DirectionalLight.reference->SetActorRotation(tempRot, ETeleportType::None);

			//Directional Light color (sun color)
			DirectionalLight.component->SetLightColor(FLinearColor::LerpUsingHSV(MorningSunColor, DaySunColor, CurrentMomentTimeNormalized));
			
		}
	}
	/////////////////////////////////////////////////

	//POINT SPOT PARAMETERS TRANSITION///////////////
	const int sizePointLights = PointSpotLights.Num();
	for (int i = 0; i < sizePointLights; i++)
	{
		if (PointSpotLights[i].reference)
		{
			if (PointSpotLights[i].component)
			{
				//If the light should be on only by night
				if (!PointSpotLights[i].component->GetShouldBeOnByDay() && PointSpotLights[i].component->GetShouldBeOnByNight())
				{
					PointSpotLights[i].reference->SetBrightness(FMath::Lerp(PointSpotLights[i].initIntensity, 0.f, CurrentMomentTimeNormalized));
				}
				//If the light should be on only by day
				else if (PointSpotLights[i].component->GetShouldBeOnByDay() && !PointSpotLights[i].component->GetShouldBeOnByNight())
				{
					PointSpotLights[i].reference->SetBrightness(FMath::Lerp(0.f, PointSpotLights[i].initIntensity, CurrentMomentTimeNormalized));
				}
			}
			
		}
	}
	/////////////////////////////////////////////////

	//SHADOW PARAMETERS TRANSITION///////////////////
	if (MorningCurveShade)
	{
		//Directional light Shadow fade
		DirectionalLight.component->SetShadowAmount(MorningCurveShade->GetFloatValue(CurrentMomentTimeNormalized));
	}
	/////////////////////////////////////////////////

	//FOG PARAMETERS TRANSITION//////////////////////
	if (MorningCurveFogDensity && MorningCurveFogHeight && HeightFogComponent)
	{
		HeightFogComponent->SetFogDensity(MorningCurveFogDensity->GetFloatValue(CurrentMomentTimeNormalized));
		HeightFogComponent->SetFogHeightFalloff(MorningCurveFogHeight->GetFloatValue(CurrentMomentTimeNormalized));

		HeightFogComponent->SetFogInscatteringColor(FLinearColor::LerpUsingHSV(MorningFogColor, DayFogColor, CurrentMomentTimeNormalized));

		HeightFogComponent->SetStartDistance(MorningFogStartDistance->GetFloatValue((CurrentMomentTimeNormalized)));
	}
	/////////////////////////////////////////////////

}

void ADayNightCycleManager::DayTransition_Implementation()
{
	//DIRECTIONAL PARAMETERS TRANSITION////////////////
	if(DirectionalLight.reference)
	{
		//Directional light angle
		FRotator tempRot = FRotator(0.f, 0.f, 0.f);
		tempRot.Pitch = FMath::Lerp(DayLightAngleBegin, DayLightAngleEnd, CurrentMomentTimeNormalized);
		DirectionalLight.reference->SetActorRotation(tempRot, ETeleportType::None);
	}
	/////////////////////////////////////////////////


	//FOG PARAMETERS TRANSITION//////////////////////
	if (DayCurveFogDensity && DayCurveFogHeight && HeightFogComponent)
	{
		HeightFogComponent->SetFogDensity(DayCurveFogDensity->GetFloatValue(CurrentMomentTimeNormalized));
		HeightFogComponent->SetFogHeightFalloff(DayCurveFogHeight->GetFloatValue(CurrentMomentTimeNormalized));

		HeightFogComponent->SetFogInscatteringColor(DayFogColor);
	}
	/////////////////////////////////////////////////
}

void ADayNightCycleManager::EveningTransition_Implementation()
{
	//DIRECTIONAL PARAMETERS TRANSITION////////////////
	if (EveningCurveFadeDirectional)
	{
		//Directional light intensity
		if(DirectionalLight.reference)
		{
			//Directional light intensity
			DirectionalLight.reference->SetBrightness(DirectionalLight.initIntensity * EveningCurveFadeDirectional->GetFloatValue(CurrentMomentTimeNormalized));

			//Directional light angle
			FRotator tempRot = FRotator(0.f, 0.f, 0.f);
			tempRot.Pitch = FMath::Lerp(DayLightAngleEnd, EveningLightAngle, CurrentMomentTimeNormalized);
			DirectionalLight.reference->SetActorRotation(tempRot, ETeleportType::None);

			//Directional Light color (sun color)
			DirectionalLight.component->SetLightColor(FLinearColor::LerpUsingHSV(DaySunColor, EveningSunColor, CurrentMomentTimeNormalized));
		}
	}
	/////////////////////////////////////////////////


	//POINT SPOT PARAMETERS TRANSITION///////////////
	int size = PointSpotLights.Num();
	for (int i = 0; i < size; i++)
	{
		if (PointSpotLights[i].reference)
		{
			if (PointSpotLights[i].component)
			{
				//If the light should be on only by night
				if (PointSpotLights[i].component->GetShouldBeOnByDay() && !PointSpotLights[i].component->GetShouldBeOnByNight())
				{
					PointSpotLights[i].reference->SetBrightness(FMath::Lerp(PointSpotLights[i].initIntensity, 0.f, CurrentMomentTimeNormalized));
				}
				//If the light should be on only by day
				else if (!PointSpotLights[i].component->GetShouldBeOnByDay() && PointSpotLights[i].component->GetShouldBeOnByNight())
				{
					PointSpotLights[i].reference->SetBrightness(FMath::Lerp(0.f, PointSpotLights[i].initIntensity, CurrentMomentTimeNormalized));
				}			
			}
		}
	}
	/////////////////////////////////////////////////

	//SHADOW PARAMETERS TRANSITION///////////////////
	if (EveningCurveShade)
	{
		//Directional light Shadow fade
		DirectionalLight.component->SetShadowAmount(EveningCurveShade->GetFloatValue(CurrentMomentTimeNormalized));
	}
	/////////////////////////////////////////////////

	//FOG PARAMETERS TRANSITION//////////////////////
	if (EveningCurveFogDensity && EveningCurveFogHeight && HeightFogComponent)
	{
		HeightFogComponent->SetFogDensity(EveningCurveFogDensity->GetFloatValue(CurrentMomentTimeNormalized));
		HeightFogComponent->SetFogHeightFalloff(EveningCurveFogHeight->GetFloatValue(CurrentMomentTimeNormalized));

		HeightFogComponent->SetFogInscatteringColor(FLinearColor::LerpUsingHSV(DayFogColor, EveningFogColor, CurrentMomentTimeNormalized));

		HeightFogComponent->SetStartDistance(EveningFogStartDistance->GetFloatValue((CurrentMomentTimeNormalized)));
	}
	/////////////////////////////////////////////////

}

void ADayNightCycleManager::NightTransition_Implementation()
{
	//FOG PARAMETERS TRANSITION//////////////////////
	if (NightCurveFogDensity && NightCurveFogHeight && HeightFogComponent)
	{
		HeightFogComponent->SetFogDensity(NightCurveFogDensity->GetFloatValue(CurrentMomentTimeNormalized));
		HeightFogComponent->SetFogHeightFalloff(NightCurveFogHeight->GetFloatValue(CurrentMomentTimeNormalized));
	}
	/////////////////////////////////////////////////
}

//Fast Forward
void ADayNightCycleManager::FastForwardTime(TYPEDayNightMoment _targetMoment, float _transitionDuration)
{
	IsCycleRunning = false;
	IsInFastForward = true;

	FastForwardTargetMoment = _targetMoment;
	FastForwardTimerMax = _transitionDuration;
	FastForwardTimer = 0.f;

	FastForwardDayInitTimeValue = DayCycleTime;
	switch (FastForwardTargetMoment)
	{
	case MORNING:
		FastForwardDayTargetTimeValue = 0.f;
		break;

	case DAY:
		FastForwardDayTargetTimeValue = (DayCycleLength *  MorningEveningProportion * DayProportion);
		break;

	case EVENING:
		FastForwardDayTargetTimeValue = (DayCycleLength *  DayProportion) - (DayCycleLength *  MorningEveningProportion * DayProportion);
		break;

	case NIGHT:
		FastForwardDayTargetTimeValue = (DayCycleLength *  DayProportion);
		break;

	default:
		break;
	}
}

void ADayNightCycleManager::FastForwardTransition(float DeltaTime)
{
	
	if (IsCycleRunning == false && IsInFastForward == true)
	{
		//Timer
		if (FastForwardTimer < 1.f)
		{
			FastForwardTimer += DeltaTime / FastForwardTimerMax;

			if (FastForwardTimer >= 1.f)
			{
				IsCycleRunning = true;
				IsInFastForward = false;
				FastForwardTimer = 1.f;
			}
			
			float timeToGoTrough = 0.f;
			//Interpolation
			if (FastForwardDayInitTimeValue < FastForwardDayTargetTimeValue)
			{
				timeToGoTrough = FastForwardDayTargetTimeValue;
			}
			else if (FastForwardDayInitTimeValue >= FastForwardDayTargetTimeValue)
			{
				timeToGoTrough = FastForwardDayTargetTimeValue + (DayCycleLength);
			}
			
			float tempDayTime = FMath::Lerp(FastForwardDayInitTimeValue, timeToGoTrough, FastForwardTimer);

			if (tempDayTime >= DayCycleLength)
			{
				tempDayTime -= DayCycleLength;
			}

			DayCycleTime = tempDayTime;
			
		}

		
		
		
	}
}
