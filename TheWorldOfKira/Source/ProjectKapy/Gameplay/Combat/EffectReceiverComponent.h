// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EffectReceiverComponent.generated.h"

class AStatusEffect;
UCLASS( ClassGroup=(Custom), Blueprintable,meta=(BlueprintSpawnableComponent) )
class PROJECTKAPY_API UEffectReceiverComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEffectReceiverComponent();

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	TArray<AStatusEffect*> StatusEffectActive;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual void AddEffect(AStatusEffect* NewEffect);

	UFUNCTION(BlueprintCallable,  meta = (ComponentClass = "AStatusEffect"), meta = (DeterminesOutputType = "NewEffect"))
	virtual AStatusEffect* AddEffectByClass(TSubclassOf<AStatusEffect> NewEffect);

	UFUNCTION(BlueprintCallable)
	virtual void ClearEffects();

	UFUNCTION(BlueprintCallable)
    virtual void RemoveEffect(AStatusEffect* Effect);
	
};
