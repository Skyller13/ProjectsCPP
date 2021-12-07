// Fill out your copyright notice in the Description page of Project Settings.


#include "VirusCharacter.h"


#include "AntiVirusTacticalCharacter.h"
#include "AntiVirusTacticalGameMode.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AVirusCharacter::AVirusCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    //Create widget component
    LifeWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Life Widget"));
    LifeWidgetComponent->SetupAttachment(RootComponent);

    //Create life system component
    lifeSystemComponent = CreateDefaultSubobject<ULifeSystemComponent>(TEXT("Life System Component"));
}

// Called when the game starts or when spawned
void AVirusCharacter::BeginPlay()
{
    Super::BeginPlay();

    gamemode = Cast<AAntiVirusTacticalGameMode>(GetWorld()->GetAuthGameMode());
}

// Called every frame
void AVirusCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (gamemode)
    {
        //is AI turn ? 
        if (!gamemode->GetTurn())
        {
            //Until random => Check if there is an allied nearby to attack him
            if (CheckEnemyNearby())
            {                
                if (currentTarget)
                {
                    if (isOnRangeToAttack)
                    {
                        //Attack
                        const auto rotator = UKismetMathLibrary::FindLookAtRotation(
                            GetActorLocation(), currentTarget->GetActorLocation());

                        SetActorRotation(rotator);

                        Attack();

                        //end turn
                        hasPlayed = true;
                        isOnRangeToAttack = false;
                        gamemode->OnEnemiesEndTurn();
                    } //attack
                    else
                    {
                        if (!currentTarget)
                            return;

                        //Chase

                        const AGridNode* targetNode = Cast<AAntiVirusTacticalCharacter>(currentTarget)->playerNode;

                        if (targetNode && LinkedNode)
                        {
                            const FVector start = LinkedNode->GetActorLocation();
                            const FVector end = targetNode->GetActorLocation();

                            const float distance = FVector::Dist(end, start);
                            float bestDistance = moveRange * 250.f;

                            AGridNode* upNode = targetNode->upNode;
                            AGridNode* upRightNode = targetNode->upRightNode;
                            AGridNode* upLeftNode = targetNode->upLeftNode;
                            AGridNode* rightNode = targetNode->RightNode;
                            AGridNode* leftNode = targetNode->leftNode;
                            AGridNode* downNode = targetNode->downNode;
                            AGridNode* downLeftNode = targetNode->downLeftNode;
                            AGridNode* downRightNde = targetNode->downRightNode;
                            AGridNode* ObjectiveNode = nullptr;

                            if (upNode && !upNode->isObstacle && upNode->LinkedActor == nullptr)
                                //up node
                            {
                                const float newDistance = FVector::Dist(upNode->GetActorLocation(), start);

                                if (newDistance < bestDistance)
                                {
                                    bestDistance = newDistance;
                                    ObjectiveNode = upNode;
                                }
                            }
                            else if (upRightNode && !upRightNode->isObstacle && upRightNode->LinkedActor == nullptr)
                                //up right node
                            {
                                const float newDistance = FVector::Dist(upRightNode->GetActorLocation(), start);

                                if (newDistance < bestDistance)
                                {
                                    bestDistance = newDistance;
                                    ObjectiveNode = upRightNode;
                                }
                            }
                            else if (upLeftNode && !upLeftNode->isObstacle && upLeftNode->LinkedActor == nullptr)
                                //up left node
                            {
                                const float newDistance = FVector::Dist(upLeftNode->GetActorLocation(), start);

                                if (newDistance < bestDistance)
                                {
                                    bestDistance = newDistance;
                                    ObjectiveNode = upLeftNode;
                                }
                            }
                            else if (rightNode && !rightNode->isObstacle && rightNode->LinkedActor == nullptr)
                                //right node
                            {
                                const float newDistance = FVector::Dist(rightNode->GetActorLocation(), start);

                                if (newDistance < bestDistance)
                                {
                                    bestDistance = newDistance;
                                    ObjectiveNode = rightNode;
                                }
                            }
                            else if (leftNode && !leftNode->isObstacle && leftNode->LinkedActor == nullptr)
                                //left node
                            {
                                const float newDistance = FVector::Dist(leftNode->GetActorLocation(), start);

                                if (newDistance < bestDistance)
                                {
                                    bestDistance = newDistance;
                                    ObjectiveNode = leftNode;
                                }
                            }
                            else if (downNode && !downNode->isObstacle && downNode->LinkedActor == nullptr)
                                //down node
                            {
                                const float newDistance = FVector::Dist(downNode->GetActorLocation(), start);

                                if (newDistance < bestDistance)
                                {
                                    bestDistance = newDistance;
                                    ObjectiveNode = downNode;
                                }
                            }
                            else if (downLeftNode && !downLeftNode->isObstacle && downLeftNode->LinkedActor == nullptr)
                                //down left node
                            {
                                const float newDistance = FVector::Dist(downLeftNode->GetActorLocation(), start);

                                if (newDistance < bestDistance)
                                {
                                    bestDistance = newDistance;
                                    ObjectiveNode = downLeftNode;
                                }
                            }
                            else if (downRightNde && !downRightNde->isObstacle && downRightNde->LinkedActor == nullptr)
                                //down right node
                            {
                                const float newDistance = FVector::Dist(downRightNde->GetActorLocation(), start);

                                if (newDistance < bestDistance)
                                {
                                    bestDistance = newDistance;
                                    ObjectiveNode = downRightNde;
                                }
                            }

                            //Face the current target
                            const auto rotator = UKismetMathLibrary::FindLookAtRotation(
                                GetActorLocation(), currentTarget->GetActorLocation());

                            SetActorRotation(rotator);

                            //move to the Objective node location
                            if (ObjectiveNode)
                            {
                                LinkedNode->LinkedActor = nullptr;
                                ObjectiveNode->LinkedActor = this;
                                
                                LinkedNode = ObjectiveNode;

                                UAIBlueprintHelperLibrary::SimpleMoveToLocation(
                                    GetController(), ObjectiveNode->GetActorLocation());
                            }

                            //end turn 
                            hasPlayed = true;
                            gamemode->OnEnemiesEndTurn();
                        }
                    } //chase
                }
            }
            else
            {
                //There is no enemy nearby the virus, 
                const int32 choice = FMath::RandRange(0, 100);

                if (choice <= 60)
                {
                    //Move !
                    const int32 randX = FMath::RandRange(-moveRange, moveRange);
                    const int32 randY = FMath::RandRange(-moveRange, moveRange);

                    RandomMovement(randX, randY);
                }
                else if (choice >= 61 && choice <= 80)
                {
                    //Spawn more virus !
                    const int32 randX = FMath::RandRange(-moveRange, moveRange);
                    const int32 randY = FMath::RandRange(-moveRange, moveRange);

                    if (gamemode->Enemies.Num() < gamemode->GetMaxEnemies())
                    {
                        SpawnVirus(randX, randY);   
                    }
                    else
                    {
                        //Wait
                    }
                }
                else //Wait
                {
                }

                hasPlayed = true;
                gamemode->OnEnemiesEndTurn();
            }
        }
    }
}

// Called to bind functionality to input
void AVirusCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}
