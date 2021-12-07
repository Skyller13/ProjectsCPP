// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(Blueprintable)
enum WeaponCategoryEnum
{
    AssaultRifles UMETA(DisplayName = "Assault"),
    SubMachineGuns UMETA(DisplayName = "SubMachine Gun"),
    SniperRifles UMETA(DisplayName = "Sniper"),
    MachineGuns UMETA(DisplayName = "Machine Gun"),
    ShotGuns UMETA(DisplayName = "Shotgun"),
    Launchers UMETA(DisplayName = "Launcher"),
    Pistols UMETA(DisplayName = "Pistol"),
    MeleeWeapons UMETA(DisplayName = "Melee"),
    Grenades UMETA(DisplayName = "Grenade"),
    Tools UMETA(DisplayName = "Tools"),
    Others UMETA(DisplayName = "Others"),
};
