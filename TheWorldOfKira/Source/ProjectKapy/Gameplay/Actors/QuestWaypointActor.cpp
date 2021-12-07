// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestWaypointActor.h"

#include "Kismet/GameplayStatics.h"
#include "ProjectKapy/Gameplay/Quest/Components/WaypointComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "ProjectKapy/ProjectKapyCharacter.h"

// Sets default values
AQuestWaypointActor::AQuestWaypointActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AQuestWaypointActor::BeginPlay()
{
	Super::BeginPlay();

	WaypointComponent = FindComponentByClass<UWaypointComponent>();

	if (WaypointComponent != nullptr)
	{
		WaypointComponent->CreateMarker();
		WaypointComponent->CreateWaypoint();
		OnDestroyed.AddUniqueDynamic(this, &AQuestWaypointActor::DestroyComponent);
	}
}

// Called every frame
void AQuestWaypointActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto character = Cast<AProjectKapyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (character != nullptr)
	{
		auto cam = character->GetCameraComponent();

		FVector facingVec = FVector::ZeroVector;
		FVector2D Vec2D = FVector2D::ZeroVector;
		
		facingValue = ValuesToUse(this, Cast<USceneComponent>(cam), Accuracy, facingVec, Vec2D);

		if (IsFacingObject(facingVec, Vec2D))
		{
			RemoveWaypointWidget_Event();
		}
		else
		{
			ModifyFacinWidget_Event();
		}
	}
}

float AQuestWaypointActor::ValuesToUse(AActor* _OtherActor, USceneComponent* _CameraComp, float _Accuracy,
	FVector& _FacingVector, FVector2D& _Vector2)
{
	_FacingVector = _CameraComp->GetForwardVector();
	auto camR = _CameraComp->GetRightVector();
	auto camLoc = _CameraComp->GetComponentLocation();
	auto ActorLoc = _OtherActor->GetRootComponent()->GetComponentLocation();

	auto rota = UKismetMathLibrary::FindLookAtRotation(camLoc, ActorLoc);
	auto RotaVec = rota.Vector();
	
	_Vector2 = FVector2D(RotaVec.X, RotaVec.Y);

	float dotValue = FVector2D::DotProduct(_Vector2, FVector2D(camR.X, camR.Y));

	//Map Range clamp
	return UKismetMathLibrary::MapRangeClamped(dotValue, (_Accuracy * - 1.f), _Accuracy, 0.f, 1.f);
}

bool AQuestWaypointActor::IsFacingObject(FVector _InVector, FVector2D _A)
{
	FVector2D tempVec2D = FVector2D(_InVector.X, _InVector.Y);

	float result = FVector2D::DotProduct(_A, tempVec2D);

	if ((result - 0.6f) >= 0.f)
	{
		return true;
	}

	return false;
}

void AQuestWaypointActor::DestroyComponent(AActor* _DestroyedActor)
{
	if (WaypointComponent)
	{
		WaypointComponent->RemoveWaypoint();
		WaypointComponent->RemoveMarker();
		WaypointComponent->DestroyComponent();
		RemoveWaypointWidget_Event();
		DestroyComponent_Event(_DestroyedActor);
	}
}


