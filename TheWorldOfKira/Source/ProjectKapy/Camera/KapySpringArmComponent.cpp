// Fill out your copyright notice in the Description page of Project Settings.


#include "KapySpringArmComponent.h"

UKapySpringArmComponent::UKapySpringArmComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Enable camera controls.
	bUsePawnControlRotation = true;

	BaseTargetArmLength = 350.0f;
	BaseSocketOffset = FVector(0.0f, 0.0f, 150.0f);
}

void UKapySpringArmComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UKapySpringArmComponent::UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag,
                                                       float DeltaTime)
{
	TargetArmLength = BaseTargetArmLength + TargetArmLengthModifier;
	SocketOffset = BaseSocketOffset + SocketOffsetModifier;
	// TargetOffset = BaseTargetOffset + TargetOffsetModifier;

	Super::UpdateDesiredArmLocation(bDoTrace, bDoLocationLag, bDoRotationLag, DeltaTime);
	TargetArmLengthModifier = 0.0f;
	SocketOffsetModifier = FVector::ZeroVector;
}
