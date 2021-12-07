// Copyright Epic Games, Inc. All Rights Reserved.

#include "DiaboloCharacter.h"

#include "DiaboloPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "LevellingSystem.h"
#include "HealthComponent.h"
#include "NavigationSystem.h"
#include "SkillDash.h"
#include "SkillManagerComponent.h"
#include "Weapon.h"
#include "WeaponComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

ADiaboloCharacter::ADiaboloCharacter()
{
    // Set size for player capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // Don't rotate character to camera direction
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
    GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
    GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->bSnapToPlaneAtStart = true;

    // Create a camera boom...
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
    CameraBoom->TargetArmLength = 1200.f;
    CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
    CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

    // Create a camera...
    TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
    TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

    // Create a decal in the world to show the cursor's location
    CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
    CursorToWorld->SetupAttachment(RootComponent);
    static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(
        TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
    if (DecalMaterialAsset.Succeeded())
    {
        CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
    }
    CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
    CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

    // Activate ticking in order to update the cursor every frame.
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

    levelSystem = CreateDefaultSubobject<ULevellingSystem>(TEXT("LevelSystem"));
    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
    WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
    SkillMgrComp = CreateDefaultSubobject<USkillManagerComponent>(TEXT("SkillManager"));
    Sk_Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Sk_Weapon"));

    if (Sk_Weapon)
    {
        Sk_Weapon->SetupAttachment(GetMesh(), TEXT("Weapon_Socket"));
    }

    CurrentSkill = nullptr;

    targetRot = CameraBoom->GetRelativeRotation();
}

void ADiaboloCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    //Camera
    CameraBoom->SetRelativeRotation(FMath::RInterpTo(CameraBoom->GetRelativeRotation(), targetRot, DeltaSeconds, 2.0f));

    // Movement
    if (!isDashing
        && currentPlayerState == Alive)
    {
        if (!MovementVectorThisFrame.IsNearlyZero() && GetController() != nullptr
            && bIsNowStanding)
        {
            MovementVectorThisFrame.Normalize(); //normalize inputs (to avoid diagonal moves to go faster)

            FVector vDestination = GetActorLocation() + (MovementVectorThisFrame * MoveSpeed * DeltaSeconds);

            //vDestination on navmesh
            vDestination = UNavigationSystemV1::GetNavigationSystem(GetWorld())->ProjectPointToNavigation(
                GetWorld(), vDestination);

            UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), vDestination);
        }
        else
        {
            //Reset sprint if Movement vector is null
            if (isRunning)
            {
                Walk();
            }
        }

        //Reload
        if (WeaponComponent
            && WeaponComponent->currentWeapon)
        {
            //Force Reload if ammo is equal to 0 and the player as enough ammo to reload his weapon
            if (WeaponComponent->currentWeapon->ammoInMagazine == 0)
            {
                if (WeaponComponent->currentWeapon->maxAmmo > 0)
                {
                    ReloadCurrentWeapon();
                }
            }
        }

        if (WeaponComponent
            && WeaponComponent->isWeaponEquipped
            && WeaponComponent->currentWeapon)
        {
            if (isRunning)
                GetCharacterMovement()->MaxWalkSpeed = (maxWalkSpeed * (WeaponComponent->currentWeapon->infos.mobility * 0.01f));
            else
                GetCharacterMovement()->MaxWalkSpeed = (maxWalkSpeed * 0.65f) * (WeaponComponent->currentWeapon->infos.mobility * 0.01f);
        }
        else
        {
            if (isRunning)
                GetCharacterMovement()->MaxWalkSpeed = maxWalkSpeed;
            else
                GetCharacterMovement()->MaxWalkSpeed = maxWalkSpeed * 0.65f;        
        }

        //orient player to character depending on animation state
        if (MovementVectorThisFrame.IsNearlyZero()
            && bIsCrouched)
        {
            OrientCharacterToMouseWhileCrouch();
        }
        else
        {
            OrientCharacterToMouse();
        }

        // Reset Movement Vector at the end of tick
        MovementVectorThisFrame = FVector::ZeroVector;
    }
}

void ADiaboloCharacter::OrientCharacterToMouse()
{
    if (GetController() == nullptr || Cast<ADiaboloPlayerController>(GetController()) == nullptr)
        return;

    FHitResult TraceHitResult;

    Cast<ADiaboloPlayerController>(GetController())->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);

    FVector impactPointZEqualsCharacterZ = FVector(TraceHitResult.ImpactPoint.X, TraceHitResult.ImpactPoint.Y,
                                                   GetActorLocation().Z);

    FRotator rot = (impactPointZEqualsCharacterZ - GetActorLocation()).ToOrientationRotator();

    SetActorRotation(rot);
}

void ADiaboloCharacter::OrientCharacterToMouseWhileCrouch()
{
    if (GetController() == nullptr || Cast<ADiaboloPlayerController>(GetController()) == nullptr)
        return;

    crouchTurnLeft = false;
    crouchTurnRight = false;

    FHitResult TraceHitResult;

    Cast<ADiaboloPlayerController>(GetController())->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);

    FVector impactPointZEqualsCharacterZ = FVector(TraceHitResult.ImpactPoint.X, TraceHitResult.ImpactPoint.Y,
                                                   GetActorLocation().Z);

    float dotValue = FVector::DotProduct(GetActorForwardVector().GetSafeNormal(),
                                         (TraceHitResult.ImpactPoint - GetActorLocation()).GetSafeNormal());
    float dotRightValue = FVector::DotProduct(GetActorRightVector().GetSafeNormal(),
                                              (TraceHitResult.ImpactPoint - GetActorLocation()).GetSafeNormal());

    if (dotValue > 0.0f)
    {
        //front of the player

        if (dotRightValue >= 0.75f)
        {
            //right
            crouchTurnRight = true;
            crouchTurnLeft = false;
        }
        else if (dotRightValue <= -0.75f)
        {
            //left
            crouchTurnLeft = true;
            crouchTurnRight = false;
        }
    }
}

void ADiaboloCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    //Movements
    InputComponent->BindAxis("MoveForward", this, &ADiaboloCharacter::OnMoveForwardAxis);
    InputComponent->BindAxis("MoveRight", this, &ADiaboloCharacter::OnMoveRightAxis);

    //Sprint
    InputComponent->BindAction("Sprint", IE_Pressed, this, &ADiaboloCharacter::Sprint);
    InputComponent->BindAction("Sprint", IE_Released, this, &ADiaboloCharacter::Walk);

    //Camera
    InputComponent->BindAction("RotateCamRight", IE_Pressed, this, &ADiaboloCharacter::RotateCameraRight);
    InputComponent->BindAction("RotateCamLeft", IE_Pressed, this, &ADiaboloCharacter::RotateCameraLeft);

    //Reload
    InputComponent->BindAction("Reloading", IE_Pressed, this, &ADiaboloCharacter::ReloadCurrentWeapon);

    //Change Weapon
    InputComponent->BindAxis("ChangeWeapon", this, &ADiaboloCharacter::SwapWeapon);

    //Equip or Unequip weapon
    InputComponent->BindAction("Equip", IE_Pressed, this, &ADiaboloCharacter::EquipUnequipWeapon);

    //Crouch
    InputComponent->BindAction("Crouch", IE_Pressed, this, &ADiaboloCharacter::Crouch);

    //Skills (Keyboard and Mouse)
    InputComponent->BindAction("LeftClickSkill", IE_Pressed, this, &ADiaboloCharacter::OnLeftMouseButtonClick);
    InputComponent->BindAction("RightClickSkill", IE_Pressed, this, &ADiaboloCharacter::OnRightMouseButtonClick);
    InputComponent->BindAction("Skill0", IE_Pressed, this, &ADiaboloCharacter::OnSkill1Input);
    InputComponent->BindAction("Skill1", IE_Pressed, this, &ADiaboloCharacter::OnSkill2Input);
    InputComponent->BindAction("Skill2", IE_Pressed, this, &ADiaboloCharacter::OnSkill3Input);
    InputComponent->BindAction("Skill3", IE_Pressed, this, &ADiaboloCharacter::OnSkill4Input);
}

void ADiaboloCharacter::OnMoveForwardAxis(const float _value)
{
    if (bIsNowStanding)
        MovementVectorThisFrame += currentForwardDirection * _value;
}

void ADiaboloCharacter::OnMoveRightAxis(const float _value)
{
    if (bIsNowStanding)
        MovementVectorThisFrame += currentRightDirection * _value;
}

void ADiaboloCharacter::Crouch()
{
    if (currentPlayerState != Alive)
        return;

    if (bIsCrouched)
    {
        bIsNowStanding = false;
        Super::UnCrouch();

        FTimerHandle UncrouchHandleTimer;
        GetWorldTimerManager().SetTimer(UncrouchHandleTimer, this, &ADiaboloCharacter::ResetMovementUncrouch, 1.35f,
                                        false);
    }
    else
    {
        bIsNowStanding = false;
        Super::Crouch();

        FTimerHandle CrouchHandleTimer;
        GetWorldTimerManager().SetTimer(CrouchHandleTimer, this, &ADiaboloCharacter::ResetMovementUncrouch, 1.0f,
                                        false);
    }
}

void ADiaboloCharacter::Sprint()
{
    if (currentPlayerState != Alive)
        return;

    isRunning = true;
}

void ADiaboloCharacter::Walk()
{
    if (currentPlayerState != Alive)
        return;

    isRunning = false;
}

void ADiaboloCharacter::ResetMovementUncrouch()
{
    bIsNowStanding = true;
}

void ADiaboloCharacter::OnSkillRequested(const int32 _skillIndex)
{
    if (SkillMgrComp && currentPlayerState == Alive)
        SkillMgrComp->OnSkillRequested(_skillIndex);
}

void ADiaboloCharacter::SetCurrentSkill(USkillBase* _skill)
{
    if (_skill)
    {
        CurrentSkill = _skill;
    }
}

USkillBase* ADiaboloCharacter::GetCurrentSkill()
{
    return CurrentSkill;
}

void ADiaboloCharacter::RotateCameraRight()
{
    auto rot = targetRot;

    targetRot = FRotator(rot.Pitch, rot.Yaw - 90.f, rot.Roll);

    currentForwardDirection = currentForwardDirection.RotateAngleAxis(-90.f, FVector(0.f, 0.f, 1.f));
    currentRightDirection = currentRightDirection.RotateAngleAxis(-90.f, FVector(0.f, 0.f, 1.f));
}

void ADiaboloCharacter::RotateCameraLeft()
{
    auto rot = targetRot;

    targetRot = FRotator(rot.Pitch, rot.Yaw + 90.f, rot.Roll);

    currentForwardDirection = currentForwardDirection.RotateAngleAxis(90.f, FVector(0.f, 0.f, 1.f));
    currentRightDirection = currentRightDirection.RotateAngleAxis(90.f, FVector(0.f, 0.f, 1.f));
}

void ADiaboloCharacter::ReloadCurrentWeapon()
{
    if (WeaponComponent)
        if (WeaponComponent->currentWeapon
            && WeaponComponent->isWeaponEquipped)
        {
            if (WeaponComponent->currentWeapon->maxAmmo > 0
                && WeaponComponent->currentWeapon->ammoInMagazine < WeaponComponent->currentWeapon->infos.ammo)
            {
                isReloading = true;

                Event_WeaponReload();

                FTimerHandle reloadTimerHandle;

                GetWorldTimerManager().SetTimer(reloadTimerHandle, this, &ADiaboloCharacter::EndReloadCurrentWeapon,
                                                WeaponComponent->currentWeapon->infos.reloadingTime * 3.30f, false);
            }
        }
}

void ADiaboloCharacter::EndReloadCurrentWeapon()
{
    if (WeaponComponent
        && WeaponComponent->currentWeapon)
    {
        isReloading = false;

        WeaponComponent->currentWeapon->maxAmmo += WeaponComponent->currentWeapon->ammoInMagazine;

        if (WeaponComponent->currentWeapon->maxAmmo >= WeaponComponent->currentWeapon->infos.ammo)
        {
            //check if there is more max ammo than ammo
            WeaponComponent->currentWeapon->ammoInMagazine = WeaponComponent->currentWeapon->infos.ammo;
            WeaponComponent->currentWeapon->maxAmmo -= WeaponComponent->currentWeapon->infos.ammo;
        }
        else
        {
            //there is less ammo than max ammo
            WeaponComponent->currentWeapon->ammoInMagazine = WeaponComponent->currentWeapon->maxAmmo;
            WeaponComponent->currentWeapon->maxAmmo = 0;
        }
    }
}

void ADiaboloCharacter::SwapWeapon(float _Value)
{
    if (WeaponComponent
        && WeaponComponent->currentWeapon
        && !isReloading
        && !isDashing
        && WeaponComponent->isWeaponEquipped)
            if (_Value > 0.f)
            {
                //new one
                if (WeaponComponent->currentWeaponIndex == WeaponComponent->inventoryWeapon.Num() - 1)
                {
                    WeaponComponent->currentWeaponIndex = 0;
                    WeaponComponent->EquipNewWeapon(WeaponComponent->inventoryWeapon[WeaponComponent->currentWeaponIndex]);
                }
                else
                {
                    WeaponComponent->currentWeaponIndex++;
                    WeaponComponent->EquipNewWeapon(WeaponComponent->inventoryWeapon[WeaponComponent->currentWeaponIndex]);
                }
            }
            else if (_Value < 0.f)
            {
                //old one
                if (WeaponComponent->currentWeaponIndex == 0)
                {
                    WeaponComponent->currentWeaponIndex = WeaponComponent->inventoryWeapon.Num() - 1;
                    WeaponComponent->EquipNewWeapon(WeaponComponent->inventoryWeapon[WeaponComponent->currentWeaponIndex]);
                }
                else
                {
                    WeaponComponent->currentWeaponIndex--;
                    WeaponComponent->EquipNewWeapon(WeaponComponent->inventoryWeapon[WeaponComponent->currentWeaponIndex]);
                }
            }
}

void ADiaboloCharacter::EquipUnequipWeapon()
{
    if (WeaponComponent)
    {
        WeaponComponent->isWeaponEquipped = !WeaponComponent->isWeaponEquipped;
        
        if (WeaponComponent->isWeaponEquipped)
        {
           //play animation equip
            EquipCurrentWeapon();
        }
        else
        {
            //play animation unequip
            UnequipCurrentWeapon();
        }
    }
}

void ADiaboloCharacter::OnLeftMouseButtonClick()
{
    OnSkillRequested(0);
}

void ADiaboloCharacter::OnRightMouseButtonClick()
{
    OnSkillRequested(1);
}

void ADiaboloCharacter::OnSkill1Input()
{
    OnSkillRequested(2);
}

void ADiaboloCharacter::OnSkill2Input()
{
    OnSkillRequested(3);
}

void ADiaboloCharacter::OnSkill3Input()
{
    OnSkillRequested(4);
}

void ADiaboloCharacter::OnSkill4Input()
{
    OnSkillRequested(5);
}
