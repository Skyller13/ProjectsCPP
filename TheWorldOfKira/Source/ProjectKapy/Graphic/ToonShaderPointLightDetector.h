// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ToonShaderPointLightDetector.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class PROJECTKAPY_API UToonShaderPointLightDetector : public UActorComponent
{
	GENERATED_BODY()

public:
	// Parameters ---------------------------- //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "Same namecode used in\n the materials(shader)"))
	FName m_ParameterNameCode = "lightDir";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "Distance to enter the pointLight\neffect area\nShould be smaller than exit distance"))
	float m_LightEnterDistance = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "Distance to exit the pointLight\neffect area\nShould be larger than enter distance"))
	float m_LightExitDistance = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "The duration you want for the\n light direction transition\nbetween 2 lights"))
	float m_DurationTransition = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "Can be manually set but it is \nnormaly automatique if not signal a bug"))
	class ADirectionalLight* m_DirectionalLight;
	// --------------------------------------- //


	// Pointer to Actors --------------------- //
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* m_Owner = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AActor*> m_ListPointLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class APointLight* m_NearestPointLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* m_PreviousLightSource;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* m_NextLightSource;

	UPROPERTY()
	class UWorld* m_World;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UMaterialInstanceDynamic*> m_Materials;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class ADayNightCycleManager* DayNightCycleManager;
	// --------------------------------------- //
	

	// Detection variables ------------------- //
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector m_NearestPointLightPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector m_OwnerPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FLinearColor m_CurrentLightDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector m_DirectionalCurrentDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool m_IsInRangeOfPointLight;
	// --------------------------------------- //



	// Transition variables -------------------//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float m_TimerTransition = 1.1f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool m_ShouldDoLightTransition = false;
	// --------------------------------------- //

	

	
	// Sets default values for this component's properties
	UToonShaderPointLightDetector();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void InitMaterials();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void CheckIfNearestIsInRange();

	UFUNCTION(BlueprintCallable)
	void GetNearest();

	UFUNCTION(BlueprintCallable)
	void ChangeLightDirection(AActor* _NextLightSource);

	UFUNCTION(BlueprintCallable)
	void TransitionLightDirection();

	UFUNCTION(BlueprintCallable)
	void PassLightDirectionToMaterial();

	UFUNCTION(BlueprintCallable)
	void SetNightColor();
		
};
