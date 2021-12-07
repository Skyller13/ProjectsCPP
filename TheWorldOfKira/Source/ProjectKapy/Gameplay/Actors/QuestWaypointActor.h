// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestWaypointActor.generated.h"

UCLASS()
class PROJECTKAPY_API AQuestWaypointActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuestWaypointActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float facingValue = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Accuracy = 0.75f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class UWaypointComponent* WaypointComponent;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	///return the current facing vector (3D and 2D) and the value clamped
	UFUNCTION(BlueprintCallable)
	float ValuesToUse(AActor* _OtherActor, USceneComponent* _CameraComp, float _Accuracy, FVector &_FacingVector, FVector2D &_Vector2);

	UFUNCTION(BlueprintCallable)
	bool IsFacingObject(FVector _InVector, FVector2D _A);

	UFUNCTION(BlueprintCallable)
	void DestroyComponent(AActor* _DestroyedActor);

	UFUNCTION(BlueprintImplementableEvent)
	void DestroyComponent_Event(AActor* _DestroyedActor);

	UFUNCTION(BlueprintImplementableEvent)
	void RemoveWaypointWidget_Event();

	UFUNCTION(BlueprintImplementableEvent)
	void ModifyFacinWidget_Event();
};
