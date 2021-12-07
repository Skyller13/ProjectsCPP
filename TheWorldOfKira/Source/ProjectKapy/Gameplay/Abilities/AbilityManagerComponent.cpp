// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityManagerComponent.h"
#include "AttackAbility.h"
#include "ProjectKapy/ProjectKapyCharacter.h"
#include "ProjectKapy/Gameplay/Combat/LifeSystemComponent.h"

// Sets default values for this component's properties
UAbilityManagerComponent::UAbilityManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// AttackAbility = NewObject<UAttackAbility>();
	// if (AttackAbility)
	// 	Abilities.Add(AttackAbility);


	// ...
}


// Called when the game starts
void UAbilityManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<UAbilityBase> subAbility : Abilities)
	{
		if (subAbility != nullptr)
		{
			AbilitiesInstanced.Add(
				NewObject<UAbilityBase>(GetOwner(), subAbility));
		}
	}
	//Init abilities Instanced
	for (UAbilityBase* ability : AbilitiesInstanced)
	{
		if (ability != nullptr)
		{
			ability->Init(this);
			OnAbilityAdded.Broadcast(ability);
		}
	}

	AProjectKapyCharacter* KapyCharacter = Cast<AProjectKapyCharacter>(GetOwner());
	if (KapyCharacter && KapyCharacter->LifeSystem)
	{
		KapyCharacter->LifeSystem->OnDamageReceived.AddDynamic(this, &UAbilityManagerComponent::OnDamage);
	}
}


// Called every frame
void UAbilityManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	for (UAbilityBase* ability : AbilitiesInstanced)
	{
		if (ability != nullptr)
		{
			ability->Tick(DeltaTime);
		}
	}
	// ...
}

void UAbilityManagerComponent::RequestAbilityPress(FName ActionName)
{

	if (bBlockRequest)
	{
		return;
	}

	for (UAbilityBase* Ability : AbilitiesInstanced)
	{
		if (Ability && Ability->ActionName == ActionName)
		{
			Ability->AbilityPressed();
			if (Ability->IsExecuting())
				OnAbilityRequestExecute.Broadcast(Ability);
			break;
		}
	}
}

void UAbilityManagerComponent::RequestAbilityRelease(FName ActionName)
{
	if (bBlockRequest)
	{
		return;
	}

	for (UAbilityBase* Ability : AbilitiesInstanced)
	{
		if (Ability && Ability->ActionName == ActionName && Ability->IsPress())
		{
			Ability->AbilityRelease();
			OnAbilityRequestExecute.Broadcast(Ability);
			break;
		}
	}
}

bool UAbilityManagerComponent::RequestAbilityExecute(FName ActionName)
{
	if (bBlockRequest)
	{
		return false;
	}

	if (IsAbilityExecuting())
	{
		return false;
	}

	for (UAbilityBase* Ability : AbilitiesInstanced)
	{
		if (Ability && Ability->ActionName == ActionName && Ability->IsExecutionValid())
		{
			Ability->Execute();
			OnAbilityRequestExecute.Broadcast(Ability);
			return true;
		}
	}
	return false;
}

bool UAbilityManagerComponent::RequestAbilityEndExecute(FName ActionName)
{
	for (UAbilityBase* Ability : AbilitiesInstanced)
	{
		if (Ability && Ability->ActionName == ActionName && Ability->IsExecuting())
		{
			Ability->EndExecute();
			return true;
		}
	}
	return false;
}

void UAbilityManagerComponent::RequestAbilityInterrupt(FName ActionName)
{
	for (UAbilityBase* Ability : AbilitiesInstanced)
	{
		if (Ability && Ability->ActionName == ActionName && Ability->IsExecuting())
		{
			Ability->Interrupt();
			break;
		}
	}
}

void UAbilityManagerComponent::DestroyComponent(bool bPromoteChildren)
{
	for (int32 i = 0; i != AbilitiesInstanced.Num(); ++i)
	{
		AbilitiesInstanced[i]->Destroy();
		AbilitiesInstanced[i]->ConditionalBeginDestroy();
		AbilitiesInstanced[i] = nullptr;
	}
	Super::OnComponentDestroyed(bPromoteChildren);
}

void UAbilityManagerComponent::InterruptAll()
{
	for (UAbilityBase* Ability : AbilitiesInstanced)
	{
		if (Ability && Ability->IsExecuting())
		{
			Ability->Interrupt();
		}
	}
}

UAbilityBase* UAbilityManagerComponent::AddAbility(UAbilityBase* NewAbility)
{
	if (NewAbility == nullptr)
	{
		return nullptr;
	}

	// TArray<UAbilityBase*> AbilitiesToRemove;

	for (int32 i = 0; i != AbilitiesInstanced.Num(); ++i)
	{
		if (AbilitiesInstanced[i]->ActionName == NewAbility->ActionName)
		{
			AbilitiesInstanced[i]->Destroy();
			AbilitiesInstanced[i]->ConditionalBeginDestroy();
			AbilitiesInstanced[i] = nullptr;
		}
	}


	AbilitiesInstanced.RemoveAll([](const UAbilityBase* Ptr)
	{
		return Ptr == nullptr;
	});

	AbilitiesInstanced.Add(NewAbility);
	NewAbility->Init(this);
	OnAbilityAdded.Broadcast(NewAbility);
	return NewAbility;
}

UAbilityBase* UAbilityManagerComponent::GetAbilityByClass(TSubclassOf<UAbilityBase> AbilityClass) const
{
	UAbilityBase* FoundAbility = nullptr;

	if (UClass* TargetClass = AbilityClass.Get())
	{
		for (UAbilityBase* Ability : AbilitiesInstanced)
		{
			if (Ability && Ability->IsA(TargetClass))
			{
				FoundAbility = Ability;
				break;
			}
		}
	}
	return FoundAbility;
}

UAbilityBase* UAbilityManagerComponent::GetAbilityByName(FName AbilityName) const
{
	for (UAbilityBase* Ability : AbilitiesInstanced)
	{
		if (Ability && Ability->ActionName == AbilityName)
		{
			return Ability;
			break;
		}
	}

	return nullptr;
}

UAbilityBase* UAbilityManagerComponent::GetAbilityExecuting() const
{
	for (UAbilityBase* Ability : AbilitiesInstanced)
	{
		if (Ability && Ability->IsExecuting())
			return Ability;
	}
	return nullptr;
}

bool UAbilityManagerComponent::IsAbilityExecuting() const
{
	for (UAbilityBase* Ability : AbilitiesInstanced)
	{
		if (Ability && Ability->IsExecuting())
			return true;
	}
	return false;
}

void UAbilityManagerComponent::SetBlockRequest(bool Value)
{
	bBlockRequest = Value;
	if (bBlockRequest && IsAbilityExecuting())
	{
		UAbilityBase* ExecutingAbility = GetAbilityExecuting();
		if (ExecutingAbility)
		{
			ExecutingAbility->Interrupt();
		}
	}
}

void UAbilityManagerComponent::OnDamage(AActor* DamagedActor, float Value, AActor* DamageCauser, EDamageType TypeDamage)
{
	for (UAbilityBase* Ability : AbilitiesInstanced)
	{
		if (Ability && Ability->bSensitiveDamage && Ability->IsDamageTypeSensitive(TypeDamage))
		{
			Ability->OnCharacterReceiveDamage(DamagedActor, Value, DamageCauser);
		}
	}
}
