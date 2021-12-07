// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillDash.h"

#include "DiaboloPlayerController.h"
#include "SkillManagerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void USkillDash::Execute()
{
    if (!SkillManagerComponent->GetOwnerCharacter()->bIsCrouched)
    {
        Super::Execute();

        if (SkillManagerComponent)
        {
            auto* player = Cast<ADiaboloCharacter>(SkillManagerComponent->GetOwnerCharacter());

            if (player)
            {
                player->SetCurrentSkill(this);
                player->isDashing = true;
            }
        }

        FTimerHandle dashHandleTimer;

        SkillManagerComponent->GetOwnerCharacter()->GetWorldTimerManager().SetTimer(
            dashHandleTimer, this, &USkillDash::Dash, dashExecute,
            false);
    }
}

void USkillDash::Dash()
{
    if (SkillManagerComponent)
    {
        SkillManagerComponent->GetOwnerCharacter()->GetCharacterMovement()->BrakingFriction = 0.0f;
        FVector direction = SkillManagerComponent->GetOwnerCharacter()->GetActorForwardVector();

        SkillManagerComponent->GetOwnerCharacter()->LaunchCharacter(
            SkillManagerComponent->GetOwnerCharacter()->GetActorForwardVector().GetSafeNormal() * range, true, true);


        FRotator lookAtRotator = FRotationMatrix::MakeFromX(direction).Rotator();

        SkillManagerComponent->GetOwnerCharacter()->SetActorRotation(FQuat::MakeFromEuler(
            FVector(SkillManagerComponent->GetOwnerCharacter()->GetActorRotation().Euler().X,
                    SkillManagerComponent->GetOwnerCharacter()->GetActorRotation().Euler().Y,
                    lookAtRotator.Euler().Z)));


        FTimerHandle dashHandleTimer;

        SkillManagerComponent->GetOwnerCharacter()->GetWorldTimerManager().SetTimer(
            dashHandleTimer, this, &USkillDash::StopDashing, dashStop,
            false);
    }
}

void USkillDash::StopDashing()
{
    if (SkillManagerComponent)
    {
        SkillManagerComponent->GetOwnerCharacter()->GetCharacterMovement()->StopMovementImmediately();
        SkillManagerComponent->GetOwnerCharacter()->GetCharacterMovement()->BrakingFriction = 2.0f;

        auto* player = Cast<ADiaboloCharacter>(SkillManagerComponent->GetOwnerCharacter());

        if (player)
        {
            player->isDashing = false;
        }
    }
}
