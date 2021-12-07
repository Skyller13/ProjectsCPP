// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetingComponent.h"

#include <Actor.h>



#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectKapy/KiraCharacter.h"
#include "ProjectKapy/Gameplay/Combat/LifeSystemComponent.h"


// Sets default values for this component's properties
UTargetingComponent::UTargetingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UTargetingComponent::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<AKiraCharacter>(GetOwner());
}


// Called every frame
void UTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Character == nullptr)
		return;

	if (IsValid(CharacterTargeted))
	{
		if (Character->isAimingMode)
		{
			FreeCharacterTargeted();
			return;
		}

		FVector ToTarget = CharacterTargeted->GetActorLocation() - Character->GetActorLocation();
		if (ToTarget.SizeSquared() > DistanceMax * DistanceMax)
		{
			TimeRemainingToFreeTarget -= DeltaTime;
			if (TimeRemainingToFreeTarget <= 0.f || ToTarget.SizeSquared() > DistanceForceFree * DistanceForceFree)
			{
				FreeCharacterTargeted();
			}
		}
		else
		{
			TimeRemainingToFreeTarget = TimeToFreeTargetOutsideRange;
		}

		if (FMath::Abs(FocusAxisConsumed) < AxisThreshold && FMath::Abs(FocusAxis) >= AxisThreshold)
		{
			SwitchTargetedActor(FocusAxis < 0.f);
		}
	}

	FocusAxisConsumed = FocusAxis;
}

TArray<AActor*> UTargetingComponent::GetAllCharacterInsideSphere(float Radius)
{
	TArray<AActor*> OutActors;
	TArray<AActor*> ActorsIgnored;
	ActorsIgnored.Add(Character);
	if (Character && !TargetObjectTypes.Num()) //Check for AI
		UKismetSystemLibrary::SphereOverlapActors(Character, Character->GetActorLocation(), Radius, TargetObjectTypes,
		                                          AProjectKapyCharacter::StaticClass(), ActorsIgnored, OutActors);
	return OutActors;
}

bool UTargetingComponent::GetActorsInFront(FVector Location, FVector Forward, const TArray<AActor*> Actors,
                                           TArray<AActor*>& OutActors)
{
	for (AActor* Actor : Actors)
	{
		FVector ToActor = Actor->GetActorLocation() - Location;

		if (FVector::DotProduct(Forward.GetSafeNormal(), ToActor.GetSafeNormal()) > 0.f)
		{
			OutActors.Add(Actor);
		}
	}
	return OutActors.Num() > 0;
}

AActor* UTargetingComponent::GetActorClosestDistanceFromArray(FVector Location, float Distance, TArray<AActor*> Actors)
{
	float DistanceMaxSqrt = Distance * Distance;
	AActor* ClosestActor = nullptr;
	for (AActor* Actor : Actors)
	{
		FVector ToActor = Actor->GetActorLocation() - Location;
		if (ToActor.SizeSquared() < DistanceMaxSqrt)
		{
			ClosestActor = Actor;
			DistanceMaxSqrt = ToActor.SizeSquared();
		}
	}
	return ClosestActor;
}

AActor* UTargetingComponent::GetActorClosestAxisFromArray(FVector Location, FVector Axis, TArray<AActor*> Actors,
                                                          float AngleMax, float Marging)
{
	if (Actors.Num() <= 0)
	{
		return nullptr;
	}

	AActor* ClosestActor = nullptr;
	float DistanceToTargetSquared = DistanceMax * DistanceMax;
	for (AActor* Actor : Actors)
	{
		FVector ToActor = Actor->GetActorLocation() - Location;
		ToActor = FVector::VectorPlaneProject(ToActor, FVector::UpVector);
		float Angle =
			FMath::RadiansToDegrees(acosf(FVector::DotProduct(Axis.GetSafeNormal(), ToActor.GetSafeNormal())));
		bool isNewTarget = false;

		UE_LOG(LogTemp, Warning, TEXT("%s : Angle %f, Lenght %f"),  *Actor->GetName(), Angle, ToActor.SizeSquared());
		
		if (Angle < AngleMax + Marging / 2.f)
		{
			if (FMath::Abs(AngleMax - Angle) < Marging)
			{
				if (ToActor.SizeSquared() < DistanceToTargetSquared)
				{
					isNewTarget = true;
				}
			}
			else
			{
				isNewTarget = true;
			}
		}

		if (isNewTarget)
		{
			ClosestActor = Actor;
			AngleMax = Angle;
			DistanceToTargetSquared = ToActor.SizeSquared();
		}
	}
	return ClosestActor;
}

AProjectKapyCharacter* UTargetingComponent::FindTargetingActorClosestAxis(
	FVector Location, FVector Forward, float Distance, float AngleMax, float AngleMarging)
{
	TArray<AActor*> OutActors = GetAllCharacterInsideSphere(Distance);
	AProjectKapyCharacter* TargetFind = nullptr;
	float DistanceToTargetSquared = Distance * Distance;
	for (AActor* Actor : OutActors)
	{
		AProjectKapyCharacter* KapyChar = Cast<AProjectKapyCharacter>(Actor);
		if (KapyChar != nullptr)
		{
			FVector ToActor = KapyChar->GetActorLocation() - Location;
			float Dot = FVector::DotProduct(Forward.GetSafeNormal(), ToActor.GetSafeNormal());
			float Angle = FMath::RadiansToDegrees(acosf(Dot));
			bool isNewTarget = false;
			if (Angle < AngleMax + AngleMarging / 2.f)
			{
				if (FMath::Abs(AngleMax - Angle) < AngleMarging)
				{
					if (ToActor.SizeSquared() < DistanceToTargetSquared)
					{
						isNewTarget = true;
					}
				}
				else
				{
					isNewTarget = true;
				}
			}

			if (isNewTarget)
			{
				TargetFind = KapyChar;
				AngleMax = Angle;
				DistanceToTargetSquared = ToActor.SizeSquared();
			}
		}
	}
	return TargetFind;
}

AProjectKapyCharacter* UTargetingComponent::SwitchTargetedActor(bool inLeft)
{
	FVector Forward = Character->GetCameraComponent()->GetForwardVector();
	FVector CameraLocation = Character->GetCameraComponent()->GetComponentLocation();
	AProjectKapyCharacter* TargetFound = nullptr;

	if (!IsValid(CharacterTargeted))
	{
		TargetFound = FindTargetingActorClosestAxis(CameraLocation, Forward,
		                                            DistanceMax, 50.f, 3.f);
		if (IsValid(TargetFound))
		{
			FocusCharacter(TargetFound);
		}
	}
	else
	{
		TArray<AActor*> OutActors = GetAllCharacterInsideSphere(DistanceMax);
		if (CharacterTargeted != nullptr && OutActors.Contains(CharacterTargeted))
		{
			OutActors.Remove(CharacterTargeted);
		}
		TArray<AActor*> OutActorsDirection;
		AActor* ClosestActor;
		GetActorsInFront(Character->GetActorLocation(), Forward, OutActors, OutActorsDirection);
		if (OutActorsDirection.Num() > 0)
		{
			FVector ToCurrentTarget = CharacterTargeted->GetActorLocation() - Character->GetCameraComponent()->
				GetComponentLocation();
			if (inLeft)
			{
				TArray<AActor*> OutLeft;
				FVector LeftVector = -ToCurrentTarget.ToOrientationQuat().GetRightVector();
				GetActorsInFront(CameraLocation, LeftVector, OutActorsDirection, OutLeft);
				ClosestActor = GetActorClosestAxisFromArray(Character->GetActorLocation(), ToCurrentTarget, OutLeft);
			}
			else
			{
				TArray<AActor*> OutRight;
				FVector RightVector = ToCurrentTarget.ToOrientationQuat().GetRightVector();
				GetActorsInFront(CameraLocation, RightVector, OutActorsDirection, OutRight);
				ClosestActor = GetActorClosestAxisFromArray(Character->GetActorLocation(), ToCurrentTarget, OutRight);
			}
			TargetFound = Cast<AProjectKapyCharacter>(ClosestActor);
		}
	}

	if (TargetFound)
	{
		if (TargetFound != CharacterTargeted)
		{
			FocusCharacter(TargetFound);
		}
	}
	return TargetFound;
}

void UTargetingComponent::FocusCharacter(AProjectKapyCharacter* _character)
{
	if (CharacterTargeted != nullptr && CharacterTargeted != _character)
	{
		FreeCharacterTargeted();
	}

	if (IsValid(_character) && _character->LifeSystem->IsAlive())
	{
		CharacterTargeted = _character;
		_character->LifeSystem->OnDeath.AddDynamic(this, &UTargetingComponent::OnTargetDied);
		CharacterTargeted->OnFocus();

		APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
		if (PlayerController)
		{
			PlayerController->SetIgnoreLookInput(true);
		}
	}

}

void UTargetingComponent::FreeCharacterTargeted()
{
	if (CharacterTargeted != nullptr && IsValid(CharacterTargeted))
	{
		CharacterTargeted->LifeSystem->OnDeath.RemoveDynamic(this, &UTargetingComponent::OnTargetDied);
		CharacterTargeted->OnDefocus();
	}
	CharacterTargeted = nullptr;

	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	if (PlayerController)
	{
		PlayerController->ResetIgnoreLookInput();
	}
	
}

void UTargetingComponent::SetFocusAxis(float _axis)
{
	FocusAxis = _axis;
}

void UTargetingComponent::OnTargetDied()
{
	TArray<AActor*> OutActors = GetAllCharacterInsideSphere(DistanceMax);
	if (CharacterTargeted != nullptr && OutActors.Contains(CharacterTargeted))
	{
		OutActors.Remove(CharacterTargeted);
	}
	FreeCharacterTargeted();

	FVector Forward = Character->GetCameraComponent()->GetForwardVector();
	Forward = FVector::VectorPlaneProject(Forward, FVector::UpVector);
	FVector CameraLocation = Character->GetCameraComponent()->GetComponentLocation();
	TArray<AActor*> OutActorsDirection;
	GetActorsInFront(Character->GetActorLocation(), Forward, OutActors, OutActorsDirection);
	AActor* TargetFound = GetActorClosestDistanceFromArray(Character->GetActorLocation(),DistanceMax, OutActorsDirection);
	if (IsValid(TargetFound))
	{
		FocusCharacter(Cast<AProjectKapyCharacter>(TargetFound));
	}
}

AProjectKapyCharacter* UTargetingComponent::GetTargetedCharacter() const
{
	return CharacterTargeted;
}
