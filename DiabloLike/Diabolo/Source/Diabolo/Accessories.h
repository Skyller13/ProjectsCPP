// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "AccessoriesCategoryEnum.h"
#include "WeaponRarityEnum.h"
#include "GameFramework/Actor.h"
#include "Accessories.generated.h"

UCLASS()
class DIABOLO_API AAccessories : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAccessories();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;
  
	//Category/ Rarity/ Cost
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type")
	TEnumAsByte<AccessoriesCategoryEnum> category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type")
	TEnumAsByte<WeaponRarityEnum> rarity;
  
	//Statistics
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistics")
	int damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistics")
	float fireRate; //bullet tick

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistics")
	float reloadingTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistics")
	float Range; //maxRange

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistics")
	int32 ammo; //max ammo in a magazine
  
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistics")
	int32 maxAmmo; //total max ammo (ammo * maxMagazine)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistics")
	int mobility; //impact the velocity of the character

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName linkedSocketName;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void AddValuesToWeapon(class AWeapon* _weapon);

	virtual void RemoveValuesToWeapon(class AWeapon* _weapon);
};
