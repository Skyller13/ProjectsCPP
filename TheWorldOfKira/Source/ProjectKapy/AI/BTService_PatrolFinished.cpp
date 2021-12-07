// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PatrolFinished.h"


#include "AIMechanicalMouseController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UBTService_PatrolFinished::UBTService_PatrolFinished(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Check if Patrol is Finish";

	bNotifyBecomeRelevant = true; 

	BlackboardKey.SelectedKeyType = UBlackboardKeyType_Bool::StaticClass();
	BlackboardKey.SelectedKeyName = FName("PatrolFinished");
	BlackboardKeyVector.SelectedKeyType = UBlackboardKeyType_Vector::StaticClass();
	BlackboardKeyVector.SelectedKeyName = FName("NextLocation");

	BlackboardKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_PatrolFinished, BlackboardKey));
	BlackboardKeyVector.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_PatrolFinished, BlackboardKeyVector));

	Interval = 0.1f;
	RandomDeviation = 0.f;
}

void UBTService_PatrolFinished::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	
	AAIMechanicalMouseController* OwnerController = Cast<AAIMechanicalMouseController>(OwnerComp.GetAIOwner());
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	Blackboard->SetValueAsBool(BlackboardKey.SelectedKeyName, false);
	if(OwnerController && Blackboard)
	{
		if(FVector::Distance(OwnerController->GetPawn()->GetActorLocation(), OwnerController->GetEndPoint()) < 150.0f)
		{
			Blackboard->SetValueAsBool(BlackboardKey.SelectedKeyName, true);
			Blackboard->SetValueAsVector(BlackboardKeyVector.SelectedKeyName, OwnerController->GetPawn()->GetActorLocation());
		}
	}
}
