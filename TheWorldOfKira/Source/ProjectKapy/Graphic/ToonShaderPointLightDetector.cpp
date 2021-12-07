// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonShaderPointLightDetector.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/PointLight.h"
#include "Engine/DirectionalLight.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "ProjectKapy/Graphic/DayNightCycleManager.h"
#include "Engine/Classes/Materials/MaterialInstanceDynamic.h"

// Sets default values for this component's properties
UToonShaderPointLightDetector::UToonShaderPointLightDetector()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UToonShaderPointLightDetector::BeginPlay()
{
	Super::BeginPlay();

	m_World = GetWorld();

	m_Owner = GetOwner();

	if (m_World && m_Owner)
	{
		UGameplayStatics::GetAllActorsOfClass(m_World, APointLight::StaticClass(), m_ListPointLight);

		if (m_ListPointLight.Num() > 0)
		{
			m_NearestPointLight = Cast<APointLight>(m_ListPointLight[0]);
		}

		m_DirectionalLight = Cast<ADirectionalLight>(
			UGameplayStatics::GetActorOfClass(m_World, ADirectionalLight::StaticClass()));

		if (m_DirectionalLight)
		{
			m_PreviousLightSource = m_DirectionalLight;

			UDirectionalLightComponent* TempDirectionalComponent = Cast<UDirectionalLightComponent>(
				m_DirectionalLight->GetComponentByClass(UDirectionalLightComponent::StaticClass()));

			if (TempDirectionalComponent)
			{
				m_CurrentLightDirection = TempDirectionalComponent->GetDirection();
			}
		}

		FTimerHandle MaterialTimerHandle;
		m_Owner->GetWorldTimerManager().SetTimer(MaterialTimerHandle, this,
		                                         &UToonShaderPointLightDetector::InitMaterials, 0.1f);

		DayNightCycleManager = Cast<ADayNightCycleManager>(
			UGameplayStatics::GetActorOfClass(m_World, ADayNightCycleManager::StaticClass()));
	}
}

void UToonShaderPointLightDetector::InitMaterials()
{
	if (m_Owner)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Owner OK !"));

		USkeletalMeshComponent* mesh = Cast<USkeletalMeshComponent>(
			m_Owner->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
		if (mesh)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Mesh OK !"));

			TArray<UMaterialInterface*> TempMaterials = mesh->GetMaterials();

			for (int i = 0; i < TempMaterials.Num(); i++)
			{
				UMaterialInstanceDynamic* tmpDynamic = mesh->CreateAndSetMaterialInstanceDynamic(i);
				if (tmpDynamic)
				{
					m_Materials.Add(tmpDynamic);
					//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Mat failed found !")); 
				}
			}
		}
	}
}


// Called every frame
void UToonShaderPointLightDetector::TickComponent(float DeltaTime, ELevelTick TickType,
                                                  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_Owner)
	{
		m_OwnerPosition = m_Owner->GetActorLocation();
	}

	if (m_NearestPointLight)
	{
		m_NearestPointLightPosition = m_NearestPointLight->GetActorLocation();
	}

#pragma region Directional Light direction update


	if (m_DirectionalLight)
	{
		UDirectionalLightComponent* TempDirectionalComponent = Cast<UDirectionalLightComponent>(
			m_DirectionalLight->GetComponentByClass(UDirectionalLightComponent::StaticClass()));

		if (TempDirectionalComponent)
		{
			m_DirectionalCurrentDirection = TempDirectionalComponent->GetDirection();
		}
	}
#pragma endregion

	GetNearest();

	CheckIfNearestIsInRange();

	TransitionLightDirection();

	PassLightDirectionToMaterial();

	SetNightColor();

	// FVector PreviousLightSourceDir = FVector(m_CurrentLightDirection.R, m_CurrentLightDirection.G, m_CurrentLightDirection.B);
	// PreviousLightSourceDir.Normalize(1.f);
	// PreviousLightSourceDir = (PreviousLightSourceDir * 200.f) + m_OwnerPosition;
	//
	// 		
	// DrawDebugLine(m_World, m_OwnerPosition, PreviousLightSourceDir,FColor::Red);
}

//Check if the nearest point light is in entering range then set has the light direction with "ChangeLightDirection"
//also check if the nearest point light is out the exit range then set the directional light has light direction
void UToonShaderPointLightDetector::CheckIfNearestIsInRange()
{
	if (m_NearestPointLight)
	{
		float dist = FVector::Dist(m_OwnerPosition, m_NearestPointLightPosition);

		if (dist < m_LightEnterDistance && m_IsInRangeOfPointLight == false)
		{
			//FVector NearestLightDir = m_OwnerPosition - m_NearestPointLightPosition;
			ChangeLightDirection(m_NearestPointLight);
			m_IsInRangeOfPointLight = true;
		}
		else if (dist > m_LightExitDistance && m_IsInRangeOfPointLight == true)
		{
			if (m_DirectionalLight)
				ChangeLightDirection(m_DirectionalLight);
			m_IsInRangeOfPointLight = false;
		}
	}
}

//Look within the point light list and set the nearest one to the pointer 
void UToonShaderPointLightDetector::GetNearest()
{
	if (m_ListPointLight.Num() > 0)
	{
		if (m_NearestPointLight)
		{
			float currentDistance = 0.f;
			float nextDistance = 0.f;

			for (int i = 0; i < m_ListPointLight.Num(); i++)
			{
				if (m_ListPointLight[i] != m_NearestPointLight)
				{
					currentDistance = FVector::Dist(m_NearestPointLight->GetActorLocation(), m_OwnerPosition);
					nextDistance = FVector::Dist(m_ListPointLight[i]->GetActorLocation(), m_OwnerPosition);

					if (nextDistance < currentDistance)
					{
						m_NearestPointLight = Cast<APointLight>(m_ListPointLight[i]);
					}
				}
			}

			if (m_PreviousLightSource == nullptr)
				m_PreviousLightSource = m_NearestPointLight;

			//if (m_NearestPointLight != m_PreviousLightSource)
			//{
			//	float dist = FVector::Dist(m_OwnerPosition, m_NearestPointLightPosition);

			//	if (dist < m_LightEnterDistance)
			//	{
			//		ChangeLightDirection(m_NearestPointLight);
			//	}
			//	/*else if (dist > m_LightExitDistance)
			//	{
			//		if (m_DirectionalLight)
			//			ChangeLightDirection(m_DirectionalLight);
			//	}*/
			//}
		}
	}
}


void UToonShaderPointLightDetector::ChangeLightDirection(AActor* _NextLightSource)
{
	if (_NextLightSource && m_PreviousLightSource)
	{
		if (_NextLightSource == m_PreviousLightSource)
			return;
	}

	if (m_PreviousLightSource == nullptr)
	{
		m_PreviousLightSource = m_DirectionalLight;
	}

	if (_NextLightSource)
	{
		if (_NextLightSource != m_PreviousLightSource)
		{
			m_NextLightSource = _NextLightSource;

			m_TimerTransition = 0.f;

			m_ShouldDoLightTransition = true;
		}
	}
}

//Smooth light direction transition within a short timer tunable
void UToonShaderPointLightDetector::TransitionLightDirection()
{
	FVector PreviousLightSourceDir;
	FVector NextLightSourceDir;

	if (m_ShouldDoLightTransition)
	{
		//Timer management
		if (m_TimerTransition < 1.f)
		{
			if (m_World)
				m_TimerTransition += m_World->GetDeltaSeconds() / m_DurationTransition;
		}
		else
		{
			m_ShouldDoLightTransition = false;
			m_PreviousLightSource = m_NextLightSource;


			return;
		}

		//Vector transition


		if (m_PreviousLightSource == m_DirectionalLight)
		{
			PreviousLightSourceDir = m_DirectionalCurrentDirection;
		}
		else
		{
			PreviousLightSourceDir = (m_OwnerPosition - m_PreviousLightSource->GetActorLocation());
			PreviousLightSourceDir.Normalize(1.f);
		}

		if (m_NextLightSource == m_DirectionalLight)
		{
			NextLightSourceDir = m_DirectionalCurrentDirection;
		}
		else
		{
			NextLightSourceDir = (m_OwnerPosition - m_NextLightSource->GetActorLocation());
			NextLightSourceDir.Normalize(1.f);
		}

		//Vector mixing formula
		m_CurrentLightDirection = (PreviousLightSourceDir * (1.f - m_TimerTransition) + (NextLightSourceDir *
			m_TimerTransition));
	}
	else
	{
		if (m_PreviousLightSource == m_DirectionalLight)
		{
			m_CurrentLightDirection = m_DirectionalCurrentDirection;
		}
		else
		{
			PreviousLightSourceDir = (m_OwnerPosition - m_PreviousLightSource->GetActorLocation());
			PreviousLightSourceDir.Normalize(1.f);
			m_CurrentLightDirection = PreviousLightSourceDir;
		}
	}
}

//Send the light direction information to the mesh materials
void UToonShaderPointLightDetector::PassLightDirectionToMaterial()
{
	for (int i = 0; i < m_Materials.Num(); i++)
	{
		if (m_Materials[i])
		{
			m_Materials[i]->SetVectorParameterValue(m_ParameterNameCode, m_CurrentLightDirection);
		}
	}
}

void UToonShaderPointLightDetector::SetNightColor()
{
	if (DayNightCycleManager && !m_IsInRangeOfPointLight)
	{
		if (DayNightCycleManager->GetDayNightMoment() == MORNING)
		{
			for (int i = 0; i < m_Materials.Num(); i++)
			{
				if (m_Materials[i])
				{
					m_Materials[i]->SetScalarParameterValue(FName("NightColorTransition"),
					                                        (1 - DayNightCycleManager->
						                                        GetDayNightMomentTimerNormalized()));
				}
			}
		}
		else if (DayNightCycleManager->GetDayNightMoment() == EVENING)
		{
			for (int i = 0; i < m_Materials.Num(); i++)
			{
				if (m_Materials[i])
				{
					m_Materials[i]->SetScalarParameterValue(FName("NightColorTransition"),
					                                        DayNightCycleManager->GetDayNightMomentTimerNormalized());
				}
			}
		}
	}
	else if (DayNightCycleManager && m_IsInRangeOfPointLight && DayNightCycleManager->GetDayNightMoment() != DAY)
	{
		float DistToLightNormalized = FMath::Clamp(FVector::Dist(m_OwnerPosition, m_PreviousLightSource->GetActorLocation()) /
            m_LightEnterDistance, 0.f, 1.f) ;

		for (int i = 0; i < m_Materials.Num(); i++)
		{
			if (m_Materials[i])
			{
				m_Materials[i]->SetScalarParameterValue(FName("NightColorTransition"), (DistToLightNormalized));
			}
		}
	}

	if (DayNightCycleManager && DayNightCycleManager->GetDayNightMoment() == DAY)
	{
		for (int i = 0; i < m_Materials.Num(); i++)
		{
			if (m_Materials[i])
			{
				m_Materials[i]->SetScalarParameterValue(FName("NightColorTransition"), 0.f);
			}
		}
	}
	else if (DayNightCycleManager && DayNightCycleManager->GetDayNightMoment() == NIGHT)
	{
		for (int i = 0; i < m_Materials.Num(); i++)
		{
			if (m_Materials[i])
			{
				m_Materials[i]->SetScalarParameterValue(FName("NightColorTransition"), 1.f);
			}
		}
	}
	
}
