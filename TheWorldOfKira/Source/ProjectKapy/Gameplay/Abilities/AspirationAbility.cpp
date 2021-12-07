// Fill out your copyright notice in the Description page of Project Settings.


#include "AspirationAbility.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectKapy/ProjectKapyCharacter.h"

UAspirationAbility::UAspirationAbility()
{
	ActionName = "Aspiration";
}

void UAspirationAbility::ClearActorAffected()
{
	CharactersAffected.RemoveAll([](const FAttractedActor Ptr)
	{
		return !IsValid(Ptr.CharRef);
	});

	TArray<AProjectKapyCharacter*> ToRemove;

	for (int32 Index = CharactersAffected.Num() - 1; Index >= 0; --Index)
	{
		if (CharactersAffected[Index].CharRef != nullptr)
		{
			FVector ToActor = Character->GetActorLocation() - CharactersAffected[Index].CharRef->GetActorLocation();
			if (ToActor.SizeSquared() > Radius * Radius)
			{
				RemoveCharacter(CharactersAffected[Index]);
				CharactersAffected.RemoveAt(Index);
			}
		}
	}
}

bool UAspirationAbility::ContainsCharacter(AProjectKapyCharacter* Char)
{
	if (Char == nullptr)
	{
		return false;
	}

	for (FAttractedActor AttractedActor : CharactersAffected)
	{
		if (AttractedActor.CharRef == Char)
		{
			return true;
		}
	}
	return false;
}

void UAspirationAbility::AddCharacter(AProjectKapyCharacter* Char)
{
	FAttractedActor AttractedActor;
	AttractedActor.CharRef = Char;
	UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(
		Char->GetMovementComponent());
	AttractedActor.BrakingDeceleration = MovementComponent->BrakingDecelerationWalking;
	AttractedActor.GroundFriction = MovementComponent->GroundFriction;

	MovementComponent->BrakingDecelerationWalking = 0.f;
	MovementComponent->GroundFriction = 4.f;

	CharactersAffected.Add(AttractedActor);
}

void UAspirationAbility::RemoveCharacter(FAttractedActor& Char)
{
	UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(
		Char.CharRef->GetMovementComponent());
	MovementComponent->BrakingDecelerationWalking = Char.BrakingDeceleration;
	MovementComponent->GroundFriction = Char.GroundFriction;
}

void UAspirationAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsExecuting)
	{
		TArray<AActor*> OutActors;
		TArray<AActor*> ActorsIgnored;
		ActorsIgnored.Add(Character);
		if (Character && !TargetObjectTypes.Num()) //Check for AI
		{
			UKismetSystemLibrary::SphereOverlapActors(Character, Character->GetActorLocation(), Radius,
			                                          TargetObjectTypes,
			                                          AProjectKapyCharacter::StaticClass(), ActorsIgnored, OutActors);

			for (AActor* actor : OutActors)
			{
				AProjectKapyCharacter* KapyCharacter = Cast<AProjectKapyCharacter>(actor);
				if (KapyCharacter)
				{
					if (!ContainsCharacter(KapyCharacter))
					{
						AddCharacter(KapyCharacter);
					}
				}
			}

			ClearActorAffected();

			for (FAttractedActor Victim : CharactersAffected)
			{
				// AProjectKapyCharacter* KapyCharacter = Victim.CharRef;
				// FVector ToActor = Character->GetActorLocation() - KapyCharacter->GetActorLocation();
				// float Factor = (FMath::Clamp(ToActor.Size(), RadiusMin, Radius) - RadiusMin) / (Radius - RadiusMin);
				// Factor = 1.f - Factor;
				// float dot = FVector::DotProduct(ToActor.GetSafeNormal(),
				//                                 KapyCharacter->GetCharacterMovement()->Velocity.GetSafeNormal());
				// float angle = FMath::RadiansToDegrees(FMath::Acos(dot)) - 90.f;
				// angle = FMath::Abs(angle);
				// float AngleFactor =  FMath::Clamp(angle / 90.f, 0.3f, 1.f);
				// FVector velocity = KapyCharacter->GetCharacterMovement()->Velocity.GetSafeNormal() * (KapyCharacter->
				// 	GetCharacterMovement()->Velocity.Size() * AngleFactor);
				//
				// velocity += ToActor.GetSafeNormal() * Strength * Factor * DeltaTime;
				// velocity.Z = KapyCharacter->GetCharacterMovement()->Velocity.Z;
				// KapyCharacter->GetCharacterMovement()->Velocity = velocity;


				AProjectKapyCharacter* KapyCharacter = Victim.CharRef;
				FVector ToActor = Character->GetActorLocation() - KapyCharacter->GetActorLocation();
				float Factor = (FMath::Clamp(ToActor.Size(), RadiusMin, Radius) - RadiusMin) / (Radius - RadiusMin);
				Factor = 1.f - Factor;
				FVector velocity = ToActor.GetSafeNormal() * Strength * Factor*  DeltaTime;
				velocity.Z *= ZFactor;
				KapyCharacter->GetMovementComponent()->Velocity += velocity;
			}
		}
	}

	if (bIsBlending)
	{
		ElapsedTime += DeltaTime;
		if (ElapsedTime >= BlendTime)
		{
			ElapsedTime = BlendTime;
			bIsBlending = false;
			BlendFactor = 1.f;
			BP_OnBlendEnded();
		}
		else
		{
			BlendFactor = ElapsedTime / BlendTime;
		}
	}
}

void UAspirationAbility::Interrupt()
{
	Super::Interrupt();
	for (FAttractedActor Victim : CharactersAffected)
	{
		RemoveCharacter(Victim);
	}
	CharactersAffected.Empty();
}

bool UAspirationAbility::IsExecutionValid() const
{
	return Super::IsExecutionValid();
}

void UAspirationAbility::SetBlend(float Time)
{
	if (Time > 0.f)
	{
		bIsBlending = true;
		BlendTime = Time;
		ElapsedTime = 0.f;
		BlendFactor = 0.f;
	}
	else
	{
		bIsBlending = false;
		BlendFactor = 1.f;
	}
}