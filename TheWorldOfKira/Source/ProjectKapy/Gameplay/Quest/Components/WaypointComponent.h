// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WaypointComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class PROJECTKAPY_API UWaypointComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWaypointComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RangeFade = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanMarkerFade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanWaypointFade;

	//Widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> Waypoint_Widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> Marker_Widget;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UUserWidget* WaypointUI;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UUserWidget* MarkerUI;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Create Waypoint in the world
	UFUNCTION(BlueprintCallable)
	void CreateWaypoint();

	//
	UFUNCTION(BlueprintImplementableEvent)
	void CreateWaypoint_Event(AActor* _actor);

	//Create a Marker in the compass
	UFUNCTION(BlueprintCallable)
	void CreateMarker();

	UFUNCTION(BlueprintImplementableEvent)
	void CreateMarker_Event(AActor* _OtherActor, USceneComponent* _CameraActor);

	UFUNCTION(BlueprintCallable)
	void AutoFadeMarker(float _range);

	UFUNCTION(BlueprintImplementableEvent)
	void ModifyImageSize(UUserWidget* _targetWidget, float _sizeX, float _sizeY);

	UFUNCTION(BlueprintCallable)
	void AutoFadeWaypoint(float _range);

	UFUNCTION(BlueprintCallable)
	void RemoveWaypoint();

	UFUNCTION(BlueprintCallable)
	void RemoveMarker();

	UFUNCTION(BlueprintImplementableEvent)
	void RemoveMarker_Event(UUserWidget* _markerUI);
	
	UFUNCTION()
	UUserWidget* GetMarkerUI();

	UFUNCTION()
	UUserWidget* GetWaypointUI();
};
