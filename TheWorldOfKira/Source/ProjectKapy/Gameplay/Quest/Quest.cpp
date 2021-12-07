// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectKapy/KiraCharacter.h"
#include "ProjectKapy/NPCCharacter.h"
#include "ProjectKapy/ProjectKapyGameMode.h"
#include "ProjectKapy/AI/SpawnerAI.h"
#include "ProjectKapy/Gameplay/Actors/SearchingAreaActor.h"
#include "ProjectKapy/Gameplay/Quest/ObjectivesData.h"
#include "ProjectKapy/Gameplay/Character/Component/QuestLogComponent.h"
#include "ProjectKapy/Gameplay/Combat/LifeSystemComponent.h"
#include "GameFramework/PlayerInput.h"
#include "ProjectKapy/Gameplay/Actors/CollectibleActor.h"

// Sets default values
AQuest::AQuest()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AQuest::BeginPlay()
{
    Super::BeginPlay();

    //Safe init
    if (QuestGiver == nullptr)
    {
        QuestGiver = GetAttachParentActor();
    }

    if (TurnInActor == nullptr)
    {
        TurnInActor = GetAttachParentActor();
    }

    bool hasLinkedSpawner = false;

    //link every spawner to his own quest !
    for (auto& Element : Objectives)
    {
        if (Element.Type == ClearArea)
        {
            if (Element.Target != nullptr)
            {
                auto Area = Cast<AKillEnemiesArea>(Element.Target);

                if (Area != nullptr)
                {
                    Area->SetQuestReference(this);
                    Area->LinkSpawnTriggerEvent();
                }
            }
        }

        if (Element.Type == Kill)
        {
            if (Element.Target != nullptr
                && Element.SpawnerList.Num() > 0)
            {
                const auto Spawner = Cast<ASpawnerAI>(Element.Target);
                
                //There is no require
                if (!Element.bRequirePreviousObjective
                     && !Element.bRequireSpecificObjective
                     && !hasLinkedSpawner)
                {
                    if (Spawner != nullptr)
                    {
                        OnSpawnTrigger.AddDynamic(this, &AQuest::SpawnTriggerKillQuest);
                        hasLinkedSpawner = true;
                    }
                }

                if (Element.bRequirePreviousObjective)
                {
                    OnObjectiveComplete.AddDynamic(this, &AQuest::SpawnTriggerObjectiveComplete);
                }

                if (Element.bRequireSpecificObjective)
                {
                    OnObjectiveComplete.AddDynamic(this, &AQuest::SpawnTriggerObjectiveComplete);
                }
            }
        }

        if (Element.Type == Collect)
        {
            if (Element.Target != nullptr)
            {
                const auto Collectible = Cast<ACollectibleActor>(Element.Target);

                if (Collectible)
                {
                    OnSpawnTrigger.AddDynamic(Collectible, &ACollectibleActor::ActivateCollectible);
                    OnQuestFail.AddDynamic(Collectible, &ACollectibleActor::DeactivateCollectible);
                }
            }
        }

        if (Element.Type == Follow || Element.Type == Chase || Element.Type == Protect)
        {
            for (auto Pos : Element.TargetList)
            {
                Element.TargetListBackup.Add(Pos);
            }
        }
    }

    //Set update timer position for waypoints and lasers
    FTimerHandle UpdateLaserPos;

    GetWorld()->GetTimerManager().SetTimer(UpdateLaserPos, this, &AQuest::UpdateMarkersPosition, 0.33f, true);
}

void AQuest::SpawnTriggerKillQuest()
{
    for (auto& Element : Objectives)
    {
        if (Element.Target != nullptr
            && Element.SpawnerList.Num() > 0
            && !Element.bRequirePreviousObjective
            && !Element.bRequireSpecificObjective)
        {
            const auto Spawner = Cast<ASpawnerAI>(Element.Target);

            if (Spawner != nullptr)
            {
                Element.TargetList = Spawner->SpawnQuest(Element.SpawnerList);
            }
        }
    }
}

void AQuest::SpawnTriggerObjectiveComplete(int32 _index)
{
    if (_index < 0 || !Objectives.IsValidIndex(_index))
        return;

    auto& currentObjective = Objectives[_index];

    if (currentObjective.Type != Kill)
        return;
    
    if (currentObjective.Target != nullptr
        && currentObjective.SpawnerList.Num() > 0)
    {
        //Check if the previous objective is done !
        if (currentObjective.bRequirePreviousObjective)
        {
            if (CheckPreviousObjective(Objectives, _index - 1))
            {
                const auto Spawner = Cast<ASpawnerAI>(currentObjective.Target);

                if (Spawner)
                {
                    currentObjective.TargetList = Spawner->SpawnQuest(currentObjective.SpawnerList);
                    return;
                }

                return;
            }
        }

        //Check if a specific objective is done !
        if (currentObjective.bRequireSpecificObjective)
        {
            if (CheckSpecificObjective(Objectives, _index, currentObjective.indexSpecificObjective))
            {
                const auto Spawner = Cast<ASpawnerAI>(currentObjective.Target);

                if (Spawner)
                {
                    currentObjective.TargetList = Spawner->SpawnQuest(currentObjective.SpawnerList);
                    return;
                }

                return;
            }
        }
    }
    else
    {
        return;
    }
}

//Update Objectives timer on each tick
void AQuest::UpdateQuestObjectives(float _DeltaTime)
{
    const auto Player = Cast<AKiraCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    if (Player == nullptr || Player->QuestLog == nullptr)
        return;

    if (bIsQuestFailed)
        return;

    if (!Player->QuestLog->Quests.Contains(this))
        return;

    for (int i = 0; i < Objectives.Num(); i++)
    {
        bool canUpdate = true;

        if (Objectives[i].MaxTimer > 0
            && !Objectives[i].bIsComplete
            && !Objectives[i].bIsFailed)
        {
            if (Objectives[i].bRequirePreviousObjective)
            {
                if (Objectives.IsValidIndex(i - 1))
                {
                    if (CheckPreviousObjective(Objectives, i - 1))
                    {
                        if (Objectives[i - 1].bIsComplete)
                        {
                            canUpdate = true;
                        }
                        else
                        {
                            canUpdate = false;
                        }
                    }
                }
            }

            if (Objectives[i].bRequireSpecificObjective)
            {
                if (Objectives.IsValidIndex(Objectives[i].indexSpecificObjective))
                {
                    if (Objectives[Objectives[i].indexSpecificObjective].bIsComplete)
                    {
                        canUpdate = true;
                    }
                    else
                    {
                        canUpdate = false;
                    }
                }
            }

            if (canUpdate)
            {
                Objectives[i].CurrentTimer += _DeltaTime;

                if (Objectives[i].CurrentTimer >= Objectives[i].MaxTimer)
                {
                    Objectives[i].CurrentTimer = Objectives[i].MaxTimer;

                    if (!Objectives[i].bIsOptional)
                    {
                        bIsQuestFailed = true;
                        Player->QuestLog->ActiveQuest = nullptr;
                        OnQuestFail.Broadcast();
                        OnQuestFailed_Event();
                        ClearAllWaypoints();
                        UpdateUI_Event();
                    }

                    if (Objectives[i].bIsOptional)
                    {
                        Objectives[i].bIsFailed = true;
                        RemoveMarkerAndWaypoint(i);
                    }
                }
            }
        }
    }
}

// Called every frame
void AQuest::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateQuestObjectives(DeltaTime);
}

void AQuest::OrganizeQuestInEditor()
{
    if (GetAttachParentActor() != nullptr)
        SetActorLocation(GetAttachParentActor()->GetActorLocation());
}

bool AQuest::CheckPreviousObjective(TArray<FObjectivesData> _Objectives, int32 _indexObjective)
{
    if (_Objectives.Num() > 0
        && _Objectives.IsValidIndex(_indexObjective))
    {
        auto element = _Objectives[_indexObjective];

        if (element.bRequirePreviousObjective)
        {
            auto previousElement = _Objectives[_indexObjective - 1];

            //previous objective complete
            if (previousElement.bIsComplete)
            {
                return true;
            }

            //previous objective undone
            return false;
        }

        //Do not require previous objective
        return true;
    }

    //Is the first object in the list or index invalid 
    return true;
}

bool AQuest::CheckSpecificObjective(TArray<FObjectivesData> _Objectives, int32 _indexObjective,
                                    int32 _indexSpecificObjective)
{
    if (_Objectives.Num() > 0
        && _Objectives.IsValidIndex(_indexObjective))
    {
        auto element = _Objectives[_indexObjective];

        if (element.bRequireSpecificObjective
            && _Objectives.IsValidIndex(_indexSpecificObjective))
        {
            if (_Objectives[_indexSpecificObjective].bIsComplete)
            {
                //Specific objective done
                return true;
            }

            //Specific objective undone
            return false;
        }

        //Do not require a specific objective
        return true;
    }

    //Do not require anything
    return true;
}

bool AQuest::PlayerHasQuest(AQuest* _quest)
{
    if (_quest != nullptr)
    {
        auto Player = Cast<AKiraCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

        if (Player)
        {
            auto quests = Player->QuestLog->Quests;

            return quests.Contains(_quest);
        }

        return false;
    }

    return false;
}

void AQuest::CheckLocationObjective(ALocationActor* _LocationActor)
{
    if (!_LocationActor)
        return;

    int index = 0;

    for (auto Element : Objectives)
    {
        if (CheckPreviousObjective(Objectives, index)
            && CheckSpecificObjective(Objectives, index, Element.indexSpecificObjective))
        {
            if (Element.Type == Location && !Element.bIsComplete
                && !Element.bIsFailed
                && Element.Target != nullptr
                && Element.Target == _LocationActor)
            {
                //Location Objective Complete
                OnObjectiveCompleteUpdateWaypoints(index);
            }
        }

        index++;
    }
}

void AQuest::CheckNPCAtLocation(AActor* _actor, ALocationActor* _LocationActor)
{
    if (!_LocationActor || !_actor)
        return;

    int index = 0;

    for (auto& Element : Objectives)
    {
        if (CheckPreviousObjective(Objectives, index)
            && CheckSpecificObjective(Objectives, index, Element.indexSpecificObjective))
        {
            //Check Quest Type
            if (Element.Type == Chase
                || Element.Type == Follow
                || Element.Type == Protect)
            {
                if (!Element.bIsComplete
                    && !Element.bIsFailed
                    && Element.Target != nullptr
                    && Element.Target == _actor)
                {
                    if (Element.TargetList.Num() > 0)
                    {
                        auto* npc = Cast<ANPCCharacter>(Element.Target);

                        if (npc != nullptr
                            && Element.TargetList[0] != nullptr)
                        {
                            if (Element.TargetList[0] == _LocationActor)
                            {
                                Element.TargetList.RemoveAt(0, 1, true);

                                if (Element.TargetList.Num() > 0)
                                {
                                    if (IsValid(Element.TargetList[0])
                                        && Element.TargetList[0] != nullptr)
                                    {
                                        npc->SetNewFinalLocation(Element.TargetList[0]->GetActorLocation());
                                    }
                                }
                                else
                                {
                                    OnObjectiveCompleteUpdateWaypoints(index);
                                    npc->SetHaveToMove(false);
                                }
                            }
                        }
                    }
                }
            }
        }

        index++;
    }
}

void AQuest::CheckInteractionObjective(AActor* _InteractedActor)
{
    if (!_InteractedActor)
        return;

    int index = 0;

    for (auto Element : Objectives)
    {
        if (CheckPreviousObjective(Objectives, index)
            && CheckSpecificObjective(Objectives, index, Element.indexSpecificObjective))
        {
            if (!Element.bIsComplete
                && !Element.bIsFailed
                && Element.Type == Interaction
                && Element.Target != nullptr
                && Element.Target == _InteractedActor)
            {
                OnObjectiveCompleteUpdateWaypoints(index);
            }
        }
        index++;
    }
}

void AQuest::CheckKilledEnemy(AActor* _enemy)
{
    if (!_enemy)
        return;

    int index = 0;

    for (auto Element : Objectives)
    {
        if (CheckPreviousObjective(Objectives, index)
            && CheckSpecificObjective(Objectives, index, Element.indexSpecificObjective))
        {
            if (!Element.bIsComplete
                && !Element.bIsFailed
                && Element.Target != nullptr
                && Element.Type == Kill)
            {
                if (Element.TargetList.Contains(_enemy))
                {
                    Objectives[index].TargetList.Remove(_enemy);
                }

                if (Objectives[index].TargetList.Num() == 0)
                {
                    //Objective complete (Kill)
                    OnObjectiveCompleteUpdateWaypoints(index);
                }
            }
        }

        index++;
    }
}

void AQuest::CheckIsPlayerListening(AActor* _actor, float _DeltaTime)
{
    if (!_actor)
        return;

    int index = 0;

    for (auto Element : Objectives)
    {
        if (CheckPreviousObjective(Objectives, index)
            && CheckSpecificObjective(Objectives, index, Element.indexSpecificObjective))
        {
            if (Element.Type == ListenConversation && !Element.bIsComplete
                && !Element.bIsFailed
                && Element.Target != nullptr
                && Element.Target == _actor)
            {
                if (Element.CurrentTimer >= Element.MaxTimer)
                {
                    Objectives[index].bIsComplete = true;
                    OnObjectiveCompleteUpdateWaypoints(index);
                }
                else
                {
                    Objectives[index].CurrentTimer += _DeltaTime;
                }
            }
        }

        index++;
    }
}

void AQuest::CheckEnemiesInArea(AKillEnemiesArea* _area)
{
    if (_area == nullptr)
        return;

    int index = 0;

    for (auto Element : Objectives)
    {
        if (CheckPreviousObjective(Objectives, index)
            && CheckSpecificObjective(Objectives, index, Element.indexSpecificObjective))
        {
            if (Element.Type == ClearArea && Element.Target != nullptr)
            {
                if (Element.Target == _area)
                {
                    OnObjectiveCompleteUpdateWaypoints(index);
                }
            }
        }

        index++;
    }
}

void AQuest::CheckProtectedNPCIsAlive()
{
    int index = 0;

    for (auto Element : Objectives)
    {
        if (CheckPreviousObjective(Objectives, index)
            && CheckSpecificObjective(Objectives, index, Element.indexSpecificObjective))
        {
            if (Element.Target != nullptr)
            {
                auto NPC = Cast<ANPCCharacter>(Element.Target);

                if (NPC)
                {
                    if (Element.Type == Protect && !Element.bIsComplete
                        && !Element.bIsFailed
                        && NPC->LifeSystemComponent->HpCurrent > 0.f)
                    {
                        OnObjectiveCompleteUpdateWaypoints(index);
                    }
                }
            }
        }
        index++;
    }
}

void AQuest::CheckScannedObjective(float _radius)
{
    const auto Player = Cast<AKiraCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    if (Player != nullptr)
    {
        for (int i = 0; i < Objectives.Num(); i++)
        {
            if (CheckPreviousObjective(Objectives, i)
                && CheckSpecificObjective(Objectives, i, Objectives[i].indexSpecificObjective))
            {
                if (Objectives[i].Type == SearchArea
                    && !Objectives[i].bIsComplete
                    && !Objectives[i].bIsFailed)
                {
                    if (Objectives[i].Target != nullptr)
                    {
                        auto searchArea = Cast<ASearchingAreaActor>(Objectives[i].Target);

                        if (searchArea != nullptr)
                        {
                            bool isComplete = true;

                            for (auto& Element : searchArea->ObjectsToScan)
                            {
                                const float distToObj = FVector::Distance(
                                    Player->GetActorLocation(), Element.Key->GetActorLocation());

                                if (!Element.Value)
                                {
                                    if (distToObj <= _radius)
                                    {
                                        Element.Value = true;
                                    }
                                    else
                                    {
                                        isComplete = false;
                                    }
                                }
                            }

                            if (isComplete)
                            {
                                OnObjectiveCompleteUpdateWaypoints(i);
                            }
                        }
                    }
                }
            }
        }
    }
}

void AQuest::OnQuestActivate()
{
    bIsActive = true;

    //Create new ones depending on the current active quest
    CreateMarkers();
    UpdateUI_Event();
}

void AQuest::OnQuestDeactivate()
{
    bIsActive = false;

    //Delete current lasers and markers from the map
    ClearAllWaypoints();
}

void AQuest::CustomObjectiveComplete(int32 _ObjectiveIndex)
{
    if (Objectives.Num() > 0
        && Objectives.IsValidIndex(_ObjectiveIndex))
    {
        auto Element = Objectives[_ObjectiveIndex];

        if (!Element.bIsComplete && !Element.bIsFailed)
        {
            OnObjectiveCompleteUpdateWaypoints(_ObjectiveIndex);
        }
    }
}

//UI Markers Waypoints

void AQuest::ClearAllWaypoints()
{
    //Clear Waypoints
    if (Waypoints.Num() > 0)
    {
        for (int i = 0; i <= Waypoints.Num(); i++)
        {
            auto waypoint = Waypoints.FindRef(i);

            if (waypoint != nullptr && IsValid(waypoint))
            {
                waypoint->Destroy();
            }
        }
    }

    //Clear Laser Markers
    if (Lasers.Num() > 0)
    {
        for (int j = 0; j <= Lasers.Num(); j++)
        {
            auto laser = Lasers.FindRef(j);

            if (laser != nullptr && IsValid(laser))
            {
                laser->Destroy();
            }
        }
    }

    //Erase all in case of doubt
    Waypoints.Empty();
    Lasers.Empty();
}

void AQuest::UpdateMarkersPosition()
{
    //Update in an update the current position of markers and lasers in the world
    if (!bIsActive)
        return;

    int index = 0;

    for (auto Element : Objectives)
    {
        if (!Element.bIsComplete && !Element.bIsFailed && Element.Target != nullptr)
        {
            if (CheckPreviousObjective(Objectives, index)
                && CheckSpecificObjective(Objectives, index,
                                          Element.indexSpecificObjective))
            {
                auto waypoint = Lasers.FindRef(index);
                auto laser = Waypoints.FindRef(index);

                if (waypoint != nullptr
                    && laser != nullptr)
                {
                    //if Element, Laser and Waypoint exists but you have to set the location on each tick (Update one)
                    FHitResult hit;
                    FVector start = Element.Target->GetActorLocation();
                    FVector end = start + FVector(0.f, 0.f, -15000.f);
                    FCollisionQueryParams QueryParams;

                    QueryParams.AddIgnoredActor(Element.Target);

                    if (GetWorld()->LineTraceSingleByChannel(
                        hit, start, end, ECC_Visibility, QueryParams))
                    {
                        //modify position !
                        Lasers[index]->SetActorLocation(hit.ImpactPoint);
                        Waypoints[index]->SetActorLocation(hit.ImpactPoint);

                        //Also check distance Laser to scale it !
                        FVector playerLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->
                            GetActorLocation();
                        FVector laserLocation = Lasers[index]->GetActorLocation();

                        float distanceToPlayer = FVector::Distance(playerLocation, laserLocation);

                        float dist = (distanceToPlayer * 0.0005f);

                        if (distanceToPlayer >= 1250.f)
                        {
                            float NewScaleZ = FMath::Clamp(dist, 0.f, 3.f);

                            Lasers[index]->SetActorScale3D(FVector(0.5f, 0.5f, NewScaleZ));
                        }
                        else
                        {
                            Lasers[index]->SetActorScale3D(FVector(0.f, 0.f, 0.f));
                        }
                    }
                }
            }
        }

        index++;
    }
}

void AQuest::OnObjectiveCompleteUpdateWaypoints(int32 _index)
{
    // Remove marker and laser depending on the current objective done
    Objectives[_index].bIsComplete = true;

    auto player = Cast<AKiraCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    if (player != nullptr)
    {
        if (player->QuestLog != nullptr)
        {
            if (player->QuestLog->ActiveQuest != nullptr
                && player->QuestLog->ActiveQuest != this)
            {
                player->QuestLog->OnActiveQuestChanged(this);
                UpdateUI_Event();
                return;
            }
        }
    }

    // Check if that should add new lasers and waypoints depending on if there is some available objectives
    int index = 0;

    for (auto Element : Objectives)
    {
        if (!Objectives[index].bIsComplete
            && !Objectives[index].bIsFailed
            && Element.Target != nullptr)
        {
            if (CheckPreviousObjective(Objectives, index)
                || CheckSpecificObjective(Objectives, index, Element.indexSpecificObjective))
            {
                auto waypoint = Waypoints.FindRef(index);
                auto laser = Lasers.FindRef(index);

                if (waypoint == nullptr && laser == nullptr)
                {
                    FHitResult hit;
                    FVector start = Element.Target->GetActorLocation();
                    FVector end = start + FVector(0.f, 0.f, -15000.f);

                    FCollisionQueryParams QueryParams;

                    QueryParams.AddIgnoredActor(Element.Target);

                    if (GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Camera, QueryParams))
                    {
                        //laser Marker
                        FTransform* transform = new FTransform(FQuat::Identity, hit.ImpactPoint,
                                                               FVector(0.5f, 0.5f, 2.f));
                        Lasers.Add(index, GetWorld()->SpawnActor(QuestLaserBP, transform));

                        //Quest Waypoint
                        FTransform* waypointTransform = new FTransform(
                            FQuat::Identity, hit.ImpactPoint, FVector(1.f, 1.f, 1.f));
                        Waypoints.Add(index, GetWorld()->SpawnActor(QuestWaypointBP, waypointTransform));
                    }

                    OnObjectiveComplete.Broadcast(index);
                }
            }
        }

        index++;
    }

    ObjectiveComplete_Event();
    RemoveMarkerAndWaypoint(_index);
    UpdateUI_Event();
}

void AQuest::CreateMarkers()
{
    //Create Markers and Lasers only
    int index = 0;

    for (auto Element : Objectives)
    {
        if (!Element.bIsComplete
            && !Element.bIsFailed
            && Element.Target != nullptr)
        {
            if (CheckPreviousObjective(Objectives, index)
                && CheckSpecificObjective(Objectives, index,
                                          Element.indexSpecificObjective))
            {
                FHitResult hit;
                FVector start = Element.Target->GetActorLocation();
                FVector end = start + FVector(0.f, 0.f, -15000.f);

                FCollisionQueryParams QueryParams;

                QueryParams.AddIgnoredActor(Element.Target);

                if (GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Camera, QueryParams))
                {
                    //laser Marker
                    FTransform* transform = new FTransform(FQuat::Identity, hit.ImpactPoint,
                                                           FVector(0.5f, 0.5f, 2.f));
                    Lasers.Add(index, GetWorld()->SpawnActor(QuestLaserBP, transform));

                    //Quest Waypoint
                    FTransform* waypointTransform = new FTransform(
                        FQuat::Identity, hit.ImpactPoint, FVector(1.f, 1.f, 1.f));
                    Waypoints.Add(index, GetWorld()->SpawnActor(QuestWaypointBP, waypointTransform));
                }
            }
        }

        index++;
    }
}

void AQuest::RemoveMarkerAndWaypoint(int32 _ObjectiveIndex)
{
    //Remove at a specific index a marker and laser related on the objective done
    if (_ObjectiveIndex >= 0
        && _ObjectiveIndex < Objectives.Num())
    {
        int i = _ObjectiveIndex;

        if ((Objectives[i].bIsComplete || Objectives[i].bIsFailed)
            && Objectives[i].Target != nullptr)
        {
            auto waypoint = Waypoints.FindRef(i);
            auto laser = Lasers.FindRef(i);

            if (laser != nullptr)
            {
                laser->Destroy();
            }

            if (waypoint != nullptr)
            {
                waypoint->Destroy();
            }
        }
    }
}

void AQuest::SetQuestToFailState()
{
    const auto Player = Cast<AKiraCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    if (Player != nullptr)
    {
        bIsQuestFailed = true;

        if (Player->QuestLog->ActiveQuest == this)
        {
            Player->QuestLog->ActiveQuest = nullptr;
        }

        OnQuestFailed_Event();
        ClearAllWaypoints();
        UpdateUI_Event();
    }
}

void AQuest::QuestDueEvent()
{
    OnQuestDue.Broadcast();
}

void AQuest::ResetFlush() const
{
    const auto Player = Cast<AKiraCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    if (Player != nullptr)
    {
        const auto Controller = Cast<APlayerController>(Player->Controller);

        if (Controller != nullptr)
        {
            Controller->PlayerInput->FlushPressedKeys();
        }
    }
}
