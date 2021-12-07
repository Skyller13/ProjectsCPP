// Fill out your copyright notice in the Description page of Project Settings.

#include "CatCharacterMovement.h"

#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"

#include "DrawDebugHelpers.h"
#include "EnumCustomMovement.h"
#include "Math/UnrealMathUtility.h"
#include <ProjectKapy\Climbable.h>


#include "ChaosInterfaceWrapperCore.h"
#include "KiraCharacter.h"
#include "Components/SplineComponent.h"
#include "Framework/MultiBox/MultiBoxDefs.h"
#include "Kismet/KismetSystemLibrary.h"

void UCatCharacterMovement::InitializeComponent()
{
    Super::InitializeComponent();
}

void UCatCharacterMovement::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (isInterpolating)
    {
        JumpInterpolation(DeltaTime);
    }
}

void UCatCharacterMovement::DoubleJump()
{
    if (doubleJumpCounter <= 1
        && MovementMode != EMovementMode::MOVE_Custom
        && !bIsPushingBlock)
    {
        if (IsFalling()
            && doubleJumpCounter == 0)
        {
            doubleJumpCounter++;
        }

        if (doubleJumpCounter < maxJumpCounter)
        {
            CharacterOwner->LaunchCharacter(FVector(0, 0, jumpHeight), false, true);
        }

        AirControl = 0.1f;

        isAbleToClimb = true;
        doubleJumpCounter++;
    }
}

void UCatCharacterMovement::JumpAgainstWall()
{
    Descent();

    FVector destination = - (((AgainstWallDirectionJump * factorJumpForce) - FVector::UpVector) * jumpDistance);

    FRotator newRotator = destination.Rotation();

    CharacterOwner->SetActorRotation(FQuat::MakeFromEuler(FVector(CharacterOwner->GetActorRotation().Euler().X,
                                                                  CharacterOwner->GetActorRotation().Euler().Y,
                                                                  newRotator.Euler().Z)));
    CharacterOwner->LaunchCharacter(destination, false, true);

    isAbleToClimb = true;
}

void UCatCharacterMovement::ResetJumpAgainstWall()
{
    isAbleToJumpAgainstWall = false;
}

void UCatCharacterMovement::PhysCustom(float deltatime, int32 Iterations)
{
    Super::PhysCustom(deltatime, Iterations);

    switch ((EmovementModeCustom)CustomMovementMode)
    {
    case EmovementModeCustom::CUSTOM_None:
        break;
    case EmovementModeCustom::CUSTOM_Climbing:
        PhysClimbing(deltatime, Iterations);
        break;
    case EmovementModeCustom::CUSTOM_JumpClimbing:
        PhysJumpClimbing(deltatime, Iterations);
        break;
    default:
        break;
    }
}

void UCatCharacterMovement::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
    if (PreviousCustomMode == EmovementModeCustom::CUSTOM_Climbing)
    {
        if (CustomMovementMode == EmovementModeCustom::CUSTOM_JumpClimbing)
        {
            AirControl = 0.0f;
        }
        else
        {
            bOrientRotationToMovement = true;
            GravityScale = 1.f;
            AirControl = 0.2f;
        }
    }

    if (PreviousCustomMode == EmovementModeCustom::CUSTOM_JumpClimbing)
    {
        if (CustomMovementMode == EmovementModeCustom::CUSTOM_Climbing)
        {
            AirControl = 0.2f;
        }
    }

    OnStateChange.Broadcast(static_cast<EmovementModeCustom>(CustomMovementMode));
}

void UCatCharacterMovement::PhysClimbing(float deltaTime, int32 Iterations)
{
    if (deltaTime < MIN_TICK_TIME)
    {
        return;
    }

    if (Acceleration.IsZero())
    {
        Velocity = FVector::ZeroVector;
    }

    CalcVelocity(deltaTime, 0.f, false, GetMaxBrakingDeceleration());

    const FVector Adjusted = Velocity * deltaTime;
    FHitResult Hit(1.f);

    SafeMoveUpdatedComponent(Adjusted, UpdatedComponent->GetComponentRotation(), true, Hit);
}

void UCatCharacterMovement::PhysJumpClimbing(float DeltaTime, int32 Iterations)
{
    if (DeltaTime < MIN_TICK_TIME)
    {
        return;
    }
}

void UCatCharacterMovement::Climb(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                  FVector NormalImpulse, const FHitResult& hit)
{
    if (OtherActor == nullptr)
    {
        return;
    }

    if (!isAbleToClimb)
    {
        return;
    }

    AClimbable* climbable = Cast<AClimbable>(OtherActor);

    if (climbable != nullptr
        && CustomMovementMode != CUSTOM_Climbing)
    {
        //Check Angle
        float scalar = FVector::DotProduct((hit.ImpactNormal.GetSafeNormal()),
                                           GetOwner()->GetActorUpVector().GetSafeNormal());
        float norm = hit.ImpactNormal.GetSafeNormal().Size() * GetOwner()->GetActorUpVector().GetSafeNormal().Size();
        float angle = FMath::RadiansToDegrees(acosf(scalar / norm));

        //Check dot product
        if (angle >= angleMin
            && angle <= angleMax)
        {
            isAbleToJumpAgainstWall = true;
            AgainstWallDirectionJump = - GetOwner()->GetActorForwardVector() - hit.ImpactNormal;
            GetOwner()->GetWorldTimerManager().SetTimer(AgainstWallTimerHandle, this,
                                                        &UCatCharacterMovement::ResetJumpAgainstWall,
                                                        timerJumpAgainstWall, false);

            //Translate
            const FVector OffSet = hit.ImpactNormal * CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius();
            const FVector NewLocation = (hit.ImpactPoint + OffSet);

            CharacterOwner->SetActorLocation(NewLocation);

            WayToLook = -hit.ImpactNormal;

            //Check scalar
            bOrientRotationToMovement = false;
            isAbleToClimb = false;

            //Rotate
            GetOwner()->SetActorRotation(WayToLook.Rotation());

            SetMovementMode(MOVE_Custom, CUSTOM_Climbing);
            GravityScale = 1.f;
            Velocity = FVector::ZeroVector;
        }
    }
}

void UCatCharacterMovement::JumpWhileClimb()
{
    if (canJumpWhileClimb)
    {
        if (MovementMode == MOVE_Custom
            && CustomMovementMode == CUSTOM_Climbing)
        {
            if (climbDirectionUp != FVector::ZeroVector
                || climbDirectionRight != FVector::ZeroVector
                && !isAbleToJumpAgainstWall)
            {
                jumpClimbDirection = climbDirectionUp + (climbDirectionRight * 0.75f);

                FVector destination = GetActorLocation() + (jumpClimbDirection * jumpDistance);

                jumpDestination = GetActorLocation();

                TArray<TEnumAsByte<EObjectTypeQuery>> ObjectQueryFilter;
                TArray<AActor*> actorsIgnored;
                FHitResult hit;

                ObjectQueryFilter.Add(
                    (TEnumAsByte<EObjectTypeQuery>)
                    UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
                ObjectQueryFilter.Add(
                    (TEnumAsByte<EObjectTypeQuery>)UEngineTypes::ConvertToObjectType(
                        ECollisionChannel::ECC_WorldDynamic));

                const bool isPlayerAble = UKismetSystemLibrary::CapsuleTraceSingleForObjects(
                    GetWorld(), destination, GetActorLocation(), 25.0f,
                    60.0f, ObjectQueryFilter, false, actorsIgnored,
                    EDrawDebugTrace::None, hit, true, FColor::Red,
                    FColor::Green, 3.0f);

                if (isPlayerAble)
                {
                    AClimbable* climbable = Cast<AClimbable>(hit.Actor);

                    if (climbable != nullptr)
                    {
                        SetMovementMode(MOVE_Custom, CUSTOM_JumpClimbing);

                        jumpDestination = hit.ImpactPoint + (hit.Normal * 60.f);

                        canJumpWhileClimb = false;

                        interpolateAlpha = 0.0f;
                        isInterpolating = true;
                    }
                }
                else
                {
                    canJumpWhileClimb = true;
                }
            }
            else
            {
                if (isAbleToJumpAgainstWall)
                {
                    JumpAgainstWall();
                }
                else
                {
                    Descent();

                    jumpDestination = GetActorLocation();

                    FVector destination = - (GetOwner()->GetActorForwardVector() * jumpDistance);

                    FRotator newRotator = destination.Rotation();

                    CharacterOwner->SetActorRotation(newRotator);
                    CharacterOwner->LaunchCharacter(destination, false, true);

                    isAbleToClimb = true;
                }
            }
        }
    }
}

void UCatCharacterMovement::JumpInterpolation(float DeltaTime)
{
    if (!canJumpWhileClimb
        && CustomMovementMode == CUSTOM_JumpClimbing)
    {
        interpolateAlpha += DeltaTime * 2.0f;

        FVector currentPos = GetActorLocation();

        if (interpolateAlpha < 1.0f)
        {
            currentPos -= GetOwner()->GetActorForwardVector() * ((1 - interpolateAlpha) * 5.0f);
        }
        else
        {
            currentPos += GetOwner()->GetActorForwardVector() * (interpolateAlpha * 2.5f);
        }

        FVector Interpolate = FMath::Lerp(currentPos, jumpDestination, DeltaTime * 2.0f);

        GetOwner()->SetActorLocation(Interpolate);

        if (interpolateAlpha > 1.5f)
        {
            isInterpolating = false;

            SetMovementMode(MOVE_Custom, CUSTOM_Climbing);

            jumpDestination = FVector(0.0f, 0.0f, 0.0f);

            GetOwner()->GetWorldTimerManager().SetTimer(cooldownJumpClimbHandle, this,
                                                        &UCatCharacterMovement::ResetJumpClimb, jumpClimbCooldown,
                                                        false);
        }
    }
}

void UCatCharacterMovement::ResetJumpClimb()
{
    canJumpWhileClimb = true;
}

void UCatCharacterMovement::Descent()
{
    if (MovementMode == MOVE_Custom
        && CustomMovementMode == CUSTOM_Climbing)
    {
        SetMovementMode(MOVE_Falling, CUSTOM_None);

        FRotator rotation = GetOwner()->GetActorRotation();

        FRotator newRotator = FRotator::ZeroRotator;

        newRotator.Yaw = rotation.Yaw;

        GetOwner()->SetActorRotation(newRotator);

        isAbleToClimb = false;
        isAbleToJumpAgainstWall = true;
    }
}