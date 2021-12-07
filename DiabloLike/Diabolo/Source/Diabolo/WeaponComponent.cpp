// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"

#include "AICharacter.h"
#include "Weapon.h"
#include "DiaboloCharacter.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
    Super::BeginPlay();
    // ...

    if (weapons.Num() > 0)
        //assign each weapon to a temporary array (inventory)
        for (auto Element : weapons)
        {
            AWeapon* outerWeapon = GetWorld()->SpawnActor<AWeapon>(Element);
            
            //Add to inventory list
            inventoryWeapon.AddUnique(outerWeapon);
        }

    if (inventoryWeapon.Num() > 0)
    {
        currentWeapon = inventoryWeapon[0];
    }

    EquipNewWeapon(currentWeapon);
}


// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

ACharacter* UWeaponComponent::GetOwnerCharacter()
{
    return Cast<ACharacter>(GetOwner());
}

void UWeaponComponent::EquipNewWeapon(AWeapon* _weapon)
{
    if (_weapon)
    {
        auto playerCharacter = Cast<ADiaboloCharacter>(GetOwnerCharacter());

        if (playerCharacter)
        {
            //Clean old accessories
            for (auto Element : tempAccessories)
            {
                Element->RemoveValuesToWeapon(currentWeapon);
                Element->Destroy();
            }

            tempAccessories.Empty();

            currentWeapon = _weapon;
            
            playerCharacter->Sk_Weapon->SetSkeletalMesh(currentWeapon->SkeletalMesh);

            //set accessories corresponding to the weapon (linked in blueprint)
            for (auto Accessories : currentWeapon->weaponAccessories)
            {
                AAccessories* outerAccessories = GetWorld()->SpawnActor<AAccessories>(Accessories);
                outerAccessories->AttachToComponent(playerCharacter->Sk_Weapon, FAttachmentTransformRules::KeepRelativeTransform, outerAccessories->linkedSocketName);
                outerAccessories->AddValuesToWeapon(currentWeapon);
                tempAccessories.AddUnique(outerAccessories);
            }
        }
    }
}
