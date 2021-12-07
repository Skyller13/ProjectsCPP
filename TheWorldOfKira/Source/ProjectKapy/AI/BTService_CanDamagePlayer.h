// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CanDamagePlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKAPY_API UBTService_CanDamagePlayer : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	UBTService_CanDamagePlayer(const FObjectInitializer& ObjectInitializer);

	protected:
	UPROPERTY(Category = Setup, EditAnywhere)
	float DistanceMax;

	public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
