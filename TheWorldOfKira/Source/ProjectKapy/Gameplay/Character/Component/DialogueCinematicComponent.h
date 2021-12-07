// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogueCinematicComponent.generated.h"


USTRUCT(BlueprintType)
struct FCameraHandle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotator;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType )
class PROJECTKAPY_API UDialogueCinematicComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, FCameraHandle> HandleMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, UAnimMontage*> AnimationMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketCinematic;
	
	// Sets default values for this component's properties
	UDialogueCinematicComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual FCameraHandle GetHandle(FString _handleName);

	UFUNCTION(BlueprintCallable)
	virtual void SetupCamera(ACameraActor* Camera, FString Handle, AActor* FocusActor);
};
