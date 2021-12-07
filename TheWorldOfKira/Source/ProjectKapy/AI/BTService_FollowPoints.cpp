// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_FollowPoints.h"

#include "AIMechanicalMouseController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UBTService_FollowPoints::UBTService_FollowPoints(const FObjectInitializer& ObjectInitializer)
     : Super(ObjectInitializer)
{
	NodeName = "Follow Points";

	bNotifyBecomeRelevant = true; 

	BlackboardKey.SelectedKeyType = UBlackboardKeyType_Vector::StaticClass();
	BlackboardKey.SelectedKeyName = FName("NextLocation");

	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_FollowPoints, BlackboardKey));

	Interval = 0.1f;
	RandomDeviation = 0.f;
}

void UBTService_FollowPoints::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	AAIMechanicalMouseController* OwnerController = Cast<AAIMechanicalMouseController>(OwnerComp.GetAIOwner());
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	
	Blackboard->SetValueAsBool(BlackboardKey.SelectedKeyName, false);
	
	if (OwnerController && Blackboard)
	{
		if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
		{
			OwnerController->GoToNextPoint(DeltaSeconds);
			Blackboard->SetValueAsVector(BlackboardKey.SelectedKeyName, OwnerController->GetFollowPoints()[OwnerController->GetCurrentPoint()]);
		}
	}
}
