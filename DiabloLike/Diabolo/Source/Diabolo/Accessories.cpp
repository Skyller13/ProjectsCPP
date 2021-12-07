// Fill out your copyright notice in the Description page of Project Settings.


#include "Accessories.h"

#include "Weapon.h"

// Sets default values
AAccessories::AAccessories()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAccessories::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAccessories::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAccessories::AddValuesToWeapon(AWeapon* _weapon)
{
	if (_weapon)
	{
		_weapon->infos.ammo += ammo;
		_weapon->infos.maxAmmo += maxAmmo;
		_weapon->infos.damage += damage;
		_weapon->infos.mobility += mobility;
		_weapon->infos.fireRate += fireRate;
		_weapon->infos.Range += Range;
		_weapon->infos.reloadingTime += reloadingTime; 
	}
}

void AAccessories::RemoveValuesToWeapon(AWeapon* _weapon)
{
	if (_weapon)
	{
		_weapon->infos.ammo -= ammo;
		_weapon->infos.maxAmmo -= maxAmmo;
		_weapon->infos.damage -= damage;
		_weapon->infos.mobility -= mobility;
		_weapon->infos.fireRate -= fireRate;
		_weapon->infos.Range -= Range;
		_weapon->infos.reloadingTime -= reloadingTime; 
	}
}

