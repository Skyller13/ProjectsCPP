// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueCinematicComponent.h"

#include "Camera/CameraActor.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UDialogueCinematicComponent::UDialogueCinematicComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDialogueCinematicComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UDialogueCinematicComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FCameraHandle UDialogueCinematicComponent::GetHandle(FString _handleName)
{
	FTransform result;
	if (HandleMap.Contains(_handleName))
	{
		return HandleMap[_handleName];
	}

	return FCameraHandle();
}

void UDialogueCinematicComponent::SetupCamera(ACameraActor* Camera, FString Handle, AActor* FocusActor)
{
	if (Camera == nullptr)
		return;

	FCameraHandle HandleStr = GetHandle(Handle);

	FTransform Transform = FTransform(HandleStr.Rotator.Quaternion(), HandleStr.Location);
	
	// HandleStr.Location += GetOwner()->GetActorLocation();
	// HandleStr.Rotator = UKismetMathLibrary::ComposeRotators(GetOwner()->GetActorRotation() , HandleStr.Rotator);


	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character)
	{
		Transform = Transform * Character->GetMesh()->GetSocketTransform(SocketCinematic);
	}
	else
	{
		Transform = Transform * GetOwner()->GetActorTransform();
	}	
	Camera->SetActorTransform(Transform);

	if (FocusActor != nullptr)
	{
		FRotator Rot = UKismetMathLibrary::FindLookAtRotation(Transform.GetLocation(), FocusActor->GetActorLocation());
		Camera->SetActorRotation(Rot);
	}
	
}
