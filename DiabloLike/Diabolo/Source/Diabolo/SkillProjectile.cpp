// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillProjectile.h"
#include "SkillManagerComponent.h"
#include "DiaboloCharacter.h"
#include "SkillLinearProjectile.h"
#include "HealthComponent.h"
#include "Weapon.h"
#include "WeaponComponent.h"

void USkillProjectile::Execute()
{
    auto character = Cast<ADiaboloCharacter>(SkillManagerComponent->GetOwnerCharacter());

    if (character)
    {
        if (character->WeaponComponent->currentWeapon
            && !character->isReloading
            && character->WeaponComponent->isWeaponEquipped)
        {
            if (character->WeaponComponent->currentWeapon->ammoInMagazine > 0)
            {
                Super::Execute();                

                //adapt cooldown spell shot with weapon fire rate
                cooldown = character->WeaponComponent->currentWeapon->infos.fireRate;
                range = character->WeaponComponent->currentWeapon->infos.Range;
                damage = character->WeaponComponent->currentWeapon->infos.damage;
            }
        }
    }
}

void USkillProjectile::TickSkill(float _DeltaTime)
{
    Super::TickSkill(_DeltaTime);
}

void USkillProjectile::SkillBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::SkillBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

    //Damage here
}

void USkillProjectile::SpawnBullet(FTransform _transform)
{
    if (SkillManagerComponent)
    {
        ASkillLinearProjectile* projectile = nullptr;
        projectile = SkillManagerComponent->GetWorld()->SpawnActorDeferred<ASkillLinearProjectile>(ProjectileBP, _transform);

        if (projectile != nullptr)
        {
            projectileRef = projectile;
            projectile->Init(this, SkillManagerComponent->GetOwnerCharacter()->GetActorForwardVector(), projectileSpeed, range);
            projectile->FinishSpawning(_transform);

            //Ammo gestion
            auto character = Cast<ADiaboloCharacter>(SkillManagerComponent->GetOwnerCharacter());

            if (character)
            {
                if (character->WeaponComponent
                    && character->WeaponComponent->currentWeapon)
                {
                    character->WeaponComponent->currentWeapon->ammoInMagazine--;
                }
            }
        }
    }
}
