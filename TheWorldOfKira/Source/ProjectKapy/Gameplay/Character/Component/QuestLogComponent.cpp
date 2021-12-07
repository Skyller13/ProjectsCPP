// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestLogComponent.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectKapy/ProjectKapyGameMode.h"
#include "ProjectKapy/Gameplay/Quest/Quest.h"

// Sets default values for this component's properties
UQuestLogComponent::UQuestLogComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}

// Called when the game starts
void UQuestLogComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UQuestLogComponent::OnActiveQuestChanged(AQuest* _NewQuest)
{
    if (ActiveQuest == nullptr || _NewQuest == nullptr)
        return;

    if (ActiveQuest != nullptr)
    {
        auto oldQuest = ActiveQuest;
        oldQuest->OnQuestDeactivate();
    }

    ActiveQuest = _NewQuest;
    _NewQuest->OnQuestActivate();
}

// Called every frame
void UQuestLogComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UQuestLogComponent::AddQuestToLog(AQuest* _Quest)
{
    if (!Quests.Contains(_Quest))
    {
        Quests.AddUnique(_Quest);
        OnActiveQuestChanged(_Quest);

        if (_Quest->bIsQuestFailed
            && _Quest->Objectives.Num() > 0)
        {
            for (int i = 0; i < _Quest->Objectives.Num(); i++)
            {
                if (_Quest->Objectives[i].TargetListBackup.Num() > 0)
                {
                    _Quest->Objectives[i].TargetList.Empty();
                    _Quest->Objectives[i].TargetList = _Quest->Objectives[i].TargetListBackup;
                }
            }
        }

        _Quest->bIsQuestFailed = false;
        _Quest->OnSpawnTrigger.Broadcast();
    }
}

void UQuestLogComponent::ToggleQuestLog()
{
    if (QuestLogUI != nullptr)
    {
        QuestLogUI->RemoveFromParent();

        QuestLogUI = nullptr;

        auto controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

        if (controller)
        {
            UWidgetBlueprintLibrary::SetInputMode_GameOnly(controller);
            controller->bShowMouseCursor = false;
        }
    }
    else
    {
        QuestLogUI = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), QuestLogWidget,
                                               TEXT("Quest Log Widget"));

        auto mode = Cast<AProjectKapyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

        if (mode)
        {
            auto slot = mode->CanvasPanel->AddChildToCanvas(QuestLogUI);

            slot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));

            const auto controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

            if (controller)
            {
                UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(controller, QuestLogUI, EMouseLockMode::LockAlways);
                controller->bShowMouseCursor = true;
            }
        }
    }
}

void UQuestLogComponent::RemoveFromQuestLog(AQuest* _Quest)
{
    if (Quests.Contains(_Quest))
    {
        Quests.Remove(_Quest);
    }
}