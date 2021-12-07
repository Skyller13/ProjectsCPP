// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Climbable.generated.h"

class UStaticMeshComponent;
class AProjectKapyCharacter;

UCLASS()
class PROJECTKAPY_API AClimbable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AClimbable();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* collisionMesh;

	UFUNCTION(BlueprintCallable)
		virtual void Climbing(AProjectKapyCharacter* _character);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
