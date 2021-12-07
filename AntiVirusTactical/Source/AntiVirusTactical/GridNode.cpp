// Fill out your copyright notice in the Description page of Project Settings.


#include "GridNode.h"

// Sets default values
AGridNode::AGridNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGridNode::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGridNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGridNode::SetRowIndex(int32 _rowIndex)
{
	if (_rowIndex >= 0)
	{
		rowIndex = _rowIndex;
	}
}

void AGridNode::SetColumnIndex(int32 _ColumnIndex)
{
	if (_ColumnIndex >= 0)
	{
		colIndex = _ColumnIndex;
	}
}