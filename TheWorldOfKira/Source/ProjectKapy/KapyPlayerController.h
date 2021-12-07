// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KapyPlayerController.generated.h"

USTRUCT(BlueprintType)
struct FCameraBlend
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* ViewTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BlendTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EViewTargetBlendFunction> BlendFunction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BlendExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StayTime;
};

USTRUCT(BlueprintType)
struct FCameraSequence
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCameraBlend> CameraBlends;
};

/**
 * 
 */
UCLASS()
class PROJECTKAPY_API AKapyPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	FCameraSequence CameraSequence;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FCameraBlend ReturnToPlayerBlend;
	int NextView;
	float TimeRemainingCameraBlend;
	bool bInBlendCamera;
public:

public:
	AKapyPlayerController();
protected:

	virtual void Tick(float DeltaSeconds) override;
	virtual void ChangeView(FCameraBlend& CameraBlend);
	virtual void SwitchCameraBlend();
	virtual void ReturnToPawnView();

public:
	UFUNCTION(BlueprintCallable)
	virtual void SetCameraSequence(const FCameraSequence _Sequence);
};
