// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetingComponent.generated.h"

class AKiraCharacter;
class AProjectKapyCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTKAPY_API UTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AProjectKapyCharacter* CharacterTargeted;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	AKiraCharacter* Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<EObjectTypeQuery>> TargetObjectTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeToFreeTargetOutsideRange = 3.f;

	float TimeRemainingToFreeTarget;

	float FocusAxis;
	
	float FocusAxisConsumed;



public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DistanceMax = 5000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AxisThreshold = 0.9f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DistanceForceFree = 6000.f;

	// Sets default values for this component's properties
	UTargetingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	virtual TArray<AActor*> GetAllCharacterInsideSphere(float Radius);
	virtual AActor* GetActorClosestDistanceFromArray(FVector Location, float Distance, TArray<AActor*> Actors);
	virtual AActor* GetActorClosestAxisFromArray(FVector Location, FVector Axis, TArray<AActor*> Actors,
	                                             float AngleMax = 90.f, float Marging = 5.f);
	virtual bool GetActorsInFront(FVector Location, FVector Forward, const TArray<AActor*> Actors,
	                              TArray<AActor*>& OutActors);

	virtual AProjectKapyCharacter* FindTargetingActorClosestAxis(FVector Location, FVector Forward, float Distance,
	                                                             float AngleMax, float AngleMarging);
	UFUNCTION(BlueprintCallable)
	virtual AProjectKapyCharacter* SwitchTargetedActor(bool inLeft);

	UFUNCTION(BlueprintCallable)
	virtual void FocusCharacter(AProjectKapyCharacter* _character);

	UFUNCTION(BlueprintCallable)
	virtual void FreeCharacterTargeted();

	UFUNCTION(BlueprintCallable)
	virtual void SetFocusAxis(float _axis);

	UFUNCTION()
	void OnTargetDied();

	AProjectKapyCharacter* GetTargetedCharacter() const;
};
