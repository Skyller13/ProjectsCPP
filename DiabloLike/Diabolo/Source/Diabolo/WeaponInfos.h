// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "Accessories.h"
#include "WeaponCategoryEnum.h"
#include "WeaponRarityEnum.h"

#include "WeaponInfos.generated.h"

USTRUCT(BlueprintType)
struct FWeaponInfos
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FText Name;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FText Description;
  
  //Category/ Rarity/ Cost
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type")
  TEnumAsByte<WeaponCategoryEnum> weaponCategory;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type")
  TEnumAsByte<WeaponRarityEnum> weaponRarity;
  
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
  int32 nbMagazines; //max magazine
  
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistics")
  int32 maxAmmo; //total max ammo (ammo * maxMagazine)

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  int mobility; //impact the velocity of the character

  //Icon
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
  UTexture2D* icon;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
  FVector2D iconSize;
};
