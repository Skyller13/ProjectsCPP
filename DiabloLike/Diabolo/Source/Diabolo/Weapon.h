// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "WeaponInfos.h"
#include "Weapon.generated.h"

UCLASS(Blueprintable, BlueprintType)
class DIABOLO_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FWeaponInfos infos;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USkeletalMesh* SkeletalMesh; //mesh of the weapon

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 ammoInMagazine; //current ammo

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 maxAmmo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    TArray<TEnumAsByte<AccessoriesCategoryEnum>> possibleModifications;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    TArray<TSubclassOf<AAccessories>> weaponAccessories;
};
