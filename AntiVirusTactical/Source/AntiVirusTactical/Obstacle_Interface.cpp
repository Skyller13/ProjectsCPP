// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle_Interface.h"

UObstacle_Interface::UObstacle_Interface()
{
}

UObstacle_Interface::~UObstacle_Interface()
{
}

void IObstacle_Interface::OnObstacleEnterEvent_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("Obstacle"));
}

