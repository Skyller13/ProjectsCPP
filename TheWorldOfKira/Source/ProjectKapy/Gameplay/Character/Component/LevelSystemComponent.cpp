// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelSystemComponent.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectKapy/ProjectKapyGameMode.h"

// Sets default values for this component's properties
ULevelSystemComponent::ULevelSystemComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}


// Called when the game starts
void ULevelSystemComponent::BeginPlay()
{
    Super::BeginPlay();

    // ...

    CalculatePercentage();

    auto mode = Cast<AProjectKapyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

    if (mode != nullptr)
    {
        SetExperienceBarWidget_Event(mode->MainWidgetUI);
    }
}


// Called every frame
void ULevelSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...

    DisplayExperienceBar(DeltaTime);

    CurrentExperience = ceil(FMath::FInterpTo(CurrentExperience, BufferExperience, DeltaTime, 5.0f));

    CalculatePercentage();

    if (CurrentExperience >= CurrentMaxExperience)
    {
        if (CurrentLevel == LevelCap)
        {
            CurrentExperience = CurrentMaxExperience;
        }
        else if (CurrentLevel < LevelCap)
        {
            LevelUp();
        
            auto mode = Cast<AProjectKapyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

            if (mode != nullptr)
            {
                LevelUp_Event(mode->MainWidgetUI);
            }
        }
    }
}

void ULevelSystemComponent::CalculatePercentage()
{
    CurrentPercentage = (CurrentExperience / CurrentMaxExperience);
}

void ULevelSystemComponent::CalculateMaxExperience(float _expFactor)
{
    CurrentMaxExperience = ceil(CurrentMaxExperience * _expFactor);
}

void ULevelSystemComponent::AddExperience(float _value, bool _bForceDisplay)
{
    if (Experience_UI)
    {
        //Visual
        Experience_UI->SetVisibility(ESlateVisibility::Visible);
        Experience_UI->SetColorAndOpacity(FLinearColor::White);

        BufferExperience += _value;
        timeRemain = 0;

        if (_bForceDisplay)
        {
            AddExperience_EventUI(_value);            
        }
    }
}

void ULevelSystemComponent::AddSkillPoint(int32 _nbSkillPoint)
{
    if (_nbSkillPoint > 0)
    {
        skillNumberRemain += _nbSkillPoint;
    }
}

void ULevelSystemComponent::LevelUp()
{
    if (CurrentLevel < LevelCap)
    {
        CurrentLevel++;
        skillNumberRemain++;

        float RemainingExperience = BufferExperience - CurrentMaxExperience;

        //reset values and calculate new max 
        CalculateMaxExperience(1.175f);
        CurrentExperience = 0.0f;
        BufferExperience = 0.0f;

        //add the remaining experience
        AddExperience(RemainingExperience, false);
    }
    else
    {
        CurrentExperience = CurrentMaxExperience;
        BufferExperience = CurrentMaxExperience;
    }
}

void ULevelSystemComponent::DisplayExperienceBar(float _DeltaTime)
{
    if (Experience_UI != nullptr
        && Experience_UI->GetVisibility() == ESlateVisibility::Visible)
    {
        timeRemain += _DeltaTime;

        if (timeRemain >= 1.5f)
        {
            FLinearColor tempColor = FLinearColor::White;

            tempColor.A = (1.f - ((timeRemain - 1.5f) / 2.1f));

            if (tempColor.A <= 0.0f)
            {
                timeRemain = 0.f;

                Experience_UI->SetVisibility(ESlateVisibility::Hidden);
            }
            else
            {
                Experience_UI->SetColorAndOpacity(tempColor);
            }
        }
    }
}
