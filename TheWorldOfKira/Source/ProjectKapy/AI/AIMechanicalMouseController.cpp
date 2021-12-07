// Fill out your copyright notice in the Description page of Project Settings.


#include "AIMechanicalMouseController.h"

#include "Containers/Array.h"
#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "ProjectKapy/ProjectKapyCharacter.h"

AAIMechanicalMouseController::AAIMechanicalMouseController()
{
}

void AAIMechanicalMouseController::BeginPlay()
{
	Super::BeginPlay();
	
	if(GetPawn())
		EndPoint = GetPawn()->GetActorLocation();
}

void AAIMechanicalMouseController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

bool AAIMechanicalMouseController::CreateFollowPoints()
{
	if(!GetPawn())
		return false;
	
	FVector ActorLocation = GetPawn()->GetActorLocation();

	CurrentPoint = 0;
	FNavLocation Location;
	ANavigationData* NavData = Cast<ANavigationData>(FNavigationSystem::GetCurrent<UNavigationSystemV1>(this)->GetMainNavData());
	if(!NavData)
		return false;
			
	const bool ResultNavData = NavData->GetRandomReachablePointInRadius(ActorLocation, MaxRadiusPatrol, Location, nullptr);
	
	EndPoint = Location.Location;
	ActorLocation.Z = EndPoint.Z;
	FollowPoints.Empty(0);
	FollowPoints.Add(ActorLocation);

	FHitResult Hit;
	const bool ResultCollision = GetWorld()->LineTraceSingleByChannel(Hit, ActorLocation, EndPoint, ECC_Camera);
				
	if(!ResultNavData && ResultCollision)
		return false;

	bool GoLeft = true;

	const FVector FirstPoint = FollowPoints.Last();
	const FVector FirstPointUnit = (EndPoint - FirstPoint).GetSafeNormal();
	const int MaxPoints = static_cast<int>(FVector::Distance(FirstPoint, EndPoint)) / static_cast<int>(MaxRadiusMovement);
                 
	for(int i = 1; i <= MaxPoints; i++)
	{
		const FVector Decale = (GoLeft ? FVector(FirstPointUnit.Y, -FirstPointUnit.X, 0.0f) * 100.0f : FVector(-FirstPointUnit.Y, FirstPointUnit.X, 0.0f) * 100.0f);
		FVector NewPoint = FirstPoint + (FirstPointUnit * MaxRadiusMovement * i) + Decale;
		FollowPoints.Add(NewPoint);
		FNavLocation NavLoc;
		bool TmpBool = UNavigationSystemV1::GetCurrent(GetWorld())->ProjectPointToNavigation(NewPoint, NavLoc, INVALID_NAVEXTENT, &FNavAgentProperties::DefaultProperties, NULL);
		if(!TmpBool)
			FollowPoints.Remove(NewPoint);
		GoLeft = !GoLeft;
	}
	FollowPoints.Add(EndPoint);

	return true;
}

void AAIMechanicalMouseController::GoToNextPoint(float DeltaSeconds)
{
	if(FollowPoints.Num() > 0)
	{
		TimerMoveTo -= DeltaSeconds;
		if(FollowPoints.Num() - 1 > CurrentPoint && FVector::Distance(GetPawn()->GetActorLocation(), FollowPoints[CurrentPoint]) <= MaxRadiusMovement || TimerMoveTo <= 0.0f)
		{
			CurrentPoint++;
			TimerMoveTo = 3.0f;
		}
		else if (FollowPoints.Num() - 1 <= CurrentPoint)
		{
			EndPoint = GetPawn()->GetActorLocation();
			TimerMoveTo = 3.0f;
		}
	}
	else
		CurrentPoint = 0;
}

bool AAIMechanicalMouseController::MoveToFollowPoints(float DeltaSeconds)
{
	if(FollowPoints.Max() >= CurrentPoint)
	{
		const EPathFollowingRequestResult::Type ResultType = MoveToLocation(FollowPoints[CurrentPoint]);
		TimerMoveTo -= DeltaSeconds;
		if(ResultType == EPathFollowingRequestResult::AlreadyAtGoal || TimerMoveTo <= 0.0f)
		{
			CurrentPoint++;
			TimerMoveTo = 3.0f;
		}
		return true;
	}

	return false;
}

