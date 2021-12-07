// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillManagerComponent.h"

#include "AICharacter.h"
#include "SkillBase.h"

// Sets default values for this component's properties
USkillManagerComponent::USkillManagerComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}


// Called when the game starts
void USkillManagerComponent::BeginPlay()
{
    Super::BeginPlay();

    // ...
    if (Skills.Num() > 0)
    {
        for (auto Element : Skills)
        {
            if (this != nullptr)
            {
                if (Element != nullptr)
                    Element->SkillManagerComponent = this;
            }
        }
    }
}


// Called every frame
void USkillManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
    if (Skills.Num() > 0)
    {
        for (auto Element : Skills)
        {
            if (Element)
                Element->TickSkill(DeltaTime);
        }
    }
}

ACharacter* USkillManagerComponent::GetOwnerCharacter() const
{
    return Cast<ACharacter>(GetOwner());
}

TArray<USkillBase*> USkillManagerComponent::GetSkills()
{
    return Skills;
}

bool USkillManagerComponent::IsASkillExecuting()
{
    if (Skills.Num() > 0)
    {
        for (auto Element : Skills)
        {
            if (Element)
                if (Element->isExecuting)
                {
                    return true;
                }
        }
    }

    return false;
}

void USkillManagerComponent::OnSkillRequested(const int32 _skillIndex)
{
    if (Skills.Num() > 0)
    {
        if (Skills.IsValidIndex(_skillIndex)
            && !IsASkillExecuting()
            && Skills[_skillIndex])
        {
            Skills[_skillIndex]->RequestExecution();
        }
    }
}
