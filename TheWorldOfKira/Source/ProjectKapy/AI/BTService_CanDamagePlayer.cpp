// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CanDamagePlayer.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"

UBTService_CanDamagePlayer::UBTService_CanDamagePlayer(const FObjectInitializer& ObjectInitializer)
     : Super(ObjectInitializer)
{
 	NodeName = "Can Attack Player Service";

	bNotifyBecomeRelevant = true; 

	BlackboardKey.SelectedKeyType = UBlackboardKeyType_Bool::StaticClass();
	BlackboardKey.SelectedKeyName = FName("CanAttackPlayer");

 	BlackboardKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_CanDamagePlayer, BlackboardKey));

 	Interval = 0.1f;
 	RandomDeviation = 0.f;
}

void UBTService_CanDamagePlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	AAIController* OwnerController = OwnerComp.GetAIOwner();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	
	Blackboard->SetValueAsBool(BlackboardKey.SelectedKeyName, false);
	
	if (OwnerController != NULL && Blackboard != NULL)
	{
		if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Bool::StaticClass())
		{
			const FVector AILocation = OwnerController->GetPawn()->GetActorLocation();
			AActor* PlayerActor = Cast<AActor>(Blackboard->GetValueAsObject("PlayerActor"));
			if(PlayerActor)
			{
				FVector ForwardAI = OwnerComp.GetAIOwner()->GetPawn()->GetActorForwardVector();
				FVector VectorToPlayer = PlayerActor->GetActorLocation() - OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
				ForwardAI = ForwardAI.GetSafeNormal();
				VectorToPlayer = VectorToPlayer.GetSafeNormal();
				
				if (FVector::Distance(AILocation, PlayerActor->GetActorLocation()) <= DistanceMax && FVector::DotProduct(ForwardAI, VectorToPlayer) >= 0.7f)
					Blackboard->SetValueAsBool(BlackboardKey.SelectedKeyName, true);
			}
		}
	}
}
