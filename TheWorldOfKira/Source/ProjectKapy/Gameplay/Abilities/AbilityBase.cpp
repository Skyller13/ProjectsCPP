// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityBase.h"

#include "AbilityActor/AbilityActorBase.h"
#include "ProjectKapy/ProjectKapyCharacter.h"

UAbilityBase::UAbilityBase()
{
}

void UAbilityBase::Init(UAbilityManagerComponent* AbilityMng)
{
	AbilityManager = AbilityMng;
	if (AbilityManager == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No AbilityManager"));
		return;
	}

	Character = Cast<AProjectKapyCharacter>(AbilityManager->GetOwner());

	BP_Init();
}

void UAbilityBase::Destroy()
{
	BP_OnDestroy();
}

void UAbilityBase::Tick(float DeltaTime)
{
	if (bIsPress)
	{
		PressElapsedTime += DeltaTime;
	}

	if (bIsExecuting)
	{
		ExecutingElapsedTime += DeltaTime;
	}

	BP_Tick(DeltaTime);
}

void UAbilityBase::AbilityPressed()
{
	bIsPress = true;
	PressElapsedTime = 0.f;
}

void UAbilityBase::AbilityRelease()
{
	bIsPress = false;
}

void UAbilityBase::Interrupt()
{
	bIsPress = false;
	bIsExecuting = false;
	ExecutingElapsedTime = 0.f;
	BP_Interrupt();
	OnInterrupt.Broadcast();
}

bool UAbilityBase::IsExecuting() const
{
	return bIsExecuting;
}

UWorld* UAbilityBase::GetWorld() const
{
	UObject *outer = GetOuter();
	if (outer && outer->IsA<AProjectKapyCharacter>() && !outer->HasAnyFlags(RF_ClassDefaultObject)) {
		return outer->GetWorld();
	}
	return nullptr;
}

bool UAbilityBase::IsDamageTypeSensitive(EDamageType _damageType)
{
	if (_damageType == EDamageType::DT_None)
	{
		return true;
	}

	for (EDamageType dt : TypeDamageSensitive)
	{
		if (dt == _damageType)
			return true;
	}
	return false;
}

void UAbilityBase::Execute()
{
	bIsExecuting = true;
	BP_Execute();
	OnExecute.Broadcast();
}

void UAbilityBase::EndExecute()
{
	if (bIsExecuting)
	{
		bIsExecuting = false;
		BP_EndExecute();
	}
}

void UAbilityBase::OnCharacterReceiveDamage(AActor* DamagedActor, float Value, AActor* DamageCauser)
{
}

bool UAbilityBase::IsExecutionValid() const
{
	return !bIsExecuting && (AbilityManager && !AbilityManager->IsAbilityExecuting());
}

void UAbilityBase::OnAbilityBeginOverlap(AAbilityActorBase* AbilityActor, UPrimitiveComponent* OverlappedComponent,
                                         AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                         bool bFromSweep,
                                         const FHitResult& SweepResult)
{
	BP_OnAbilityBeginOverlap(AbilityActor, OverlappedComponent, OtherActor,
	                         OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void UAbilityBase::OnAbilityEndOverlap(AAbilityActorBase* AbilityActor, UPrimitiveComponent* OverlappedComponent,
                                       AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	BP_OnAbilityEndOverlap(AbilityActor, OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void UAbilityBase::OnAbilityHit(AAbilityActorBase* AbilityActor, UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	BP_OnAbilityHit(AbilityActor, HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
}

AAbilityActorBase* UAbilityBase::InstantiateAbilityActor(TSubclassOf<AAbilityActorBase> AbilityActor)
{
	AAbilityActorBase* ActorSpawned = AbilityManager->GetWorld()->SpawnActorDeferred<AAbilityActorBase>(
		AbilityActor, Character->GetActorTransform(), Character);
	ActorSpawned->Init(this);
	ActorSpawned->FinishSpawning(Character->GetActorTransform());
	ActorSpawned->AttachToActor(Character, FAttachmentTransformRules::KeepWorldTransform);
	return ActorSpawned;
}
