// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/SplineComponent.h"
#include "EnumCustomMovement.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CatCharacterMovement.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMovementStateChangeSignature, EmovementModeCustom, CustomMovement);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDashDelegate);
//
UCLASS(Blueprintable)
class PROJECTKAPY_API UCatCharacterMovement : public UCharacterMovementComponent
{
    GENERATED_BODY()

protected :

    virtual void InitializeComponent() override;

public :
    ///Properties

    UCapsuleComponent* capsuleComponent;

    UPROPERTY(BlueprintAssignable)
    FMovementStateChangeSignature OnStateChange;
    
#pragma region JumpProperties

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 doubleJumpCounter = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 maxJumpCounter = 2;

    UPROPERTY(EditAnywhere)
    float jumpHeight = 600.0f;

    bool isAbleToJumpAgainstWall = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float timerJumpAgainstWall = 0.5f;
    
    FTimerHandle AgainstWallTimerHandle;

    FVector AgainstWallDirectionJump;

    UPROPERTY(EditAnywhere)
    int factorJumpForce = 2.0f;

#pragma endregion

// #pragma region DashProperties
//
//     UPROPERTY(EditAnywhere)
//     float dashDistance = 6000.0f;
//
//     UPROPERTY(EditAnywhere)
//     float dashCooldown = 2.0f;
//
//     UPROPERTY()
//     bool canDash = true;
//
//     UPROPERTY(EditAnywhere)
//     float dashStop = 0.1f;
//
//     UPROPERTY()
//     FVector playerVelocityUntilDash = FVector::ZeroVector;
//
//     UPROPERTY()
//     FTimerHandle unusedHandle;
//
// 	UPROPERTY(BlueprintAssignable)
// 	FDashDelegate OnDash;
//
// #pragma endregion

#pragma region ClimbProperties

    UPROPERTY(VisibleAnywhere, Category = "Climb")
    bool isAbleToClimb = true;

    UPROPERTY(VisibleAnywhere)
    FVector WayToLook = FVector::ZeroVector;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climb")
    float angleMin = 40.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climb")
    float angleMax = 125.f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climb")
    bool CanClimbLeft;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climb")
    bool CanClimbRight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climb")
    bool isOnEdge; //check above the player if he can reach the top

    //jump while climbing properties

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JumpClimb")
    bool canJumpWhileClimb = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "JumpClimb")
    float jumpClimbCooldown = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "JumpClimb")
    float jumpDistance = 600.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JumpClimb")
    FVector jumpClimbDirection = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JumpClimb")
    FVector jumpDestination = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "JumpClimb")
    FVector climbDirectionUp = FVector::ZeroVector;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "JumpClimb")
    FVector climbDirectionRight = FVector::ZeroVector;

    UPROPERTY()
    FTimerHandle cooldownJumpClimbHandle;

    bool isInterpolating = false;

    float interpolateAlpha = 0.0f;
    
#pragma endregion

#pragma region PushBlockProperties

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsPushingBlock = false;
    
#pragma endregion
    
//Functions

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    
#pragma region JumpFunctions

    UFUNCTION(BlueprintCallable)
    void DoubleJump();

    UFUNCTION()
    void JumpAgainstWall();

    UFUNCTION()
    void ResetJumpAgainstWall();
    
#pragma endregion

// #pragma region DashFunctions
//
//     void Dash();
//
//     void StopDashing();
//
//     void ResetDash();
//
// #pragma endregion

#pragma region ClimbFunctions

    virtual void PhysCustom(float DeltaTime, int32 Iterations) override;
    virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

    virtual void PhysClimbing(float DeltaTime, int32 Iterations);

    UFUNCTION()
    void Climb(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
               FVector NormalImpulse, const FHitResult& hit);
    
    UFUNCTION(BlueprintCallable)
    void Descent();
    
#pragma endregion
    
#pragma region jumpClimbFunctions
    
    virtual void PhysJumpClimbing(float DeltaTime, int32 Iterations);
    
    UFUNCTION()
  void JumpWhileClimb();

    UFUNCTION()
    void JumpInterpolation(float DeltaTime);

    UFUNCTION()
    void ResetJumpClimb();
    
#pragma endregion
};
