// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridNode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNodeSelectSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNodeUnselectSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNodeResetSignature);

UCLASS()
class ANTIVIRUSTACTICAL_API AGridNode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridNode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:

	UPROPERTY(BlueprintGetter = GetRowIndex)
	int32 rowIndex;

	UPROPERTY(BlueprintGetter = GetColumnIndex)
	int32 colIndex;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isObstacle = false;
	
	//Select node (display)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
	FOnNodeSelectSignature OnNodeSelected;

	//Unselect node (display)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
	FOnNodeUnselectSignature OnNodeUnselected;

	//Reset node (range display) 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
	FOnNodeResetSignature OnNodeReset;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#pragma region Nodes
	///Nodes
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* LinkedActor = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AGridNode* upNode = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AGridNode* upRightNode = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AGridNode* RightNode = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AGridNode* downRightNode = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AGridNode* downNode = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AGridNode* downLeftNode = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AGridNode* leftNode = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AGridNode* upLeftNode = nullptr;
#pragma endregion
	
	//Methods
	UFUNCTION(BlueprintCallable)
	int32 GetRowIndex() const {return rowIndex;}

	UFUNCTION(BlueprintCallable)
	int32 GetColumnIndex() const {return colIndex;}

	UFUNCTION(BlueprintCallable)
	void SetRowIndex(int32 _rowIndex);

	UFUNCTION(BlueprintCallable)
	void SetColumnIndex(int32 _ColumnIndex);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetNodeHighlighted();
};
