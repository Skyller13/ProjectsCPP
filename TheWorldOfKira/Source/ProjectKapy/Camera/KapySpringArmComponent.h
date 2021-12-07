// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "KapySpringArmComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=Camera, meta=(BlueprintSpawnableComponent), hideCategories=(Mobility))
class PROJECTKAPY_API UKapySpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseTargetArmLength;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	float TargetArmLengthModifier;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector BaseSocketOffset;
	FVector SocketOffsetModifier;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector BaseTargetOffset;
	FVector TargetOffsetModifier;

public:
	UKapySpringArmComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;

	virtual void UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag,
	                                      float DeltaTime) override;
};
