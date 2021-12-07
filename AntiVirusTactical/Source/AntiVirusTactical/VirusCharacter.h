// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridNode.h"
#include "LifeSystemComponent.h"
#include "GameFramework/Character.h"
#include "VirusCharacter.generated.h"

UCLASS()
class ANTIVIRUSTACTICAL_API AVirusCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVirusCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool hasPlayed = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AGridNode* LinkedNode = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AAntiVirusTacticalGameMode* gamemode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* currentTarget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isOnRangeToAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int moveRange = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UWidgetComponent* LifeWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ULifeSystemComponent* lifeSystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 damage;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool CheckEnemyNearby();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Attack();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void RandomMovement(int32 _randX, int32 _randY);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SpawnVirus(int32 _posX, int32 _posY);
};