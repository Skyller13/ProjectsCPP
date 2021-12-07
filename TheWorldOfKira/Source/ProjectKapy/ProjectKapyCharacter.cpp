// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectKapyCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "CatCharacterMovement.h"
#include "WallClimbable.h"
#include "Camera/KapySpringArmComponent.h"
#include "Gameplay/Abilities/JumpAbility.h"
#include "Gameplay/Combat/EffectReceiverComponent.h"
#include "Graphic/DayNightCycleManager.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectKapy/Gameplay/Combat/LifeSystemComponent.h"
#include "ProjectKapy/Gameplay/Combat/ManaSystemComponent.h"
#include "ProjectKapy/Gameplay/Abilities/AbilityManagerComponent.h"
#include "Graphic/ToonShaderPointLightDetector.h"


//////////////////////////////////////////////////////////////////////////
// AProjectKapyCharacter

AProjectKapyCharacter::AProjectKapyCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCatCharacterMovement>(
		ACharacter::CharacterMovementComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	WalkSpeed = 275.f;
	RunSpeed = 525.f;

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.15f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraSpringArm = CreateDefaultSubobject<UKapySpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraSpringArm->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraSpringArm->bEditableWhenInherited = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	// Use this custom character movement
	CatCharacterMovement = Cast<UCatCharacterMovement>(GetMovementComponent());

	LifeSystem = CreateDefaultSubobject<ULifeSystemComponent>(TEXT("LifeSystem"));
	ManaSystem = CreateDefaultSubobject<UManaSystemComponent>(TEXT("ManaSystem"));
	AbilityManager = CreateDefaultSubobject<UAbilityManagerComponent>(TEXT("AbilityManager"));
	EffectReceiver = CreateDefaultSubobject<UEffectReceiverComponent>(TEXT("EffectReceiver"));
	ShaderComponent = CreateDefaultSubobject<UToonShaderPointLightDetector>(TEXT("ToonShaderComponent"));
	CharacterState = ECharacterState::STATE_None;
}


void AProjectKapyCharacter::BeginPlay()
{
	Super::BeginPlay();


	if (LifeSystem)
	{
		LifeSystem->OnDeath.AddDynamic(this, &AProjectKapyCharacter::OnDeath);
		LifeSystem->OnRevive.AddDynamic(this, &AProjectKapyCharacter::OnRevive);
	}

	// if (GetCapsuleComponent() != nullptr)
	// {
	//     if (CatCharacterMovement)
	//         CatCharacterMovement->capsuleComponent = GetCapsuleComponent();
	//
	//     GetCapsuleComponent()->OnComponentHit.AddUniqueDynamic(this, &AProjectKapyCharacter::Climb);
	// }

	if (CatCharacterMovement)
	{
		CatCharacterMovement->OnStateChange.AddDynamic(this, &AProjectKapyCharacter::OnMovementModeCustomChanged);
		CatCharacterMovement->MaxWalkSpeed = RunSpeed;
	}

	WalkSpeedBase = WalkSpeed;
	RunSpeedBase = RunSpeed;
	SetSpeedModifier(SpeedModifier);
	SetCharacterState(ECharacterState::STATE_None);
}

void AProjectKapyCharacter::Tick(float Deltatime)
{
	Super::Tick(Deltatime);
	if (bInTimeDilation)
	{
		TimeDilationDuration -= Deltatime;
		if (TimeDilationDuration <= 0.f)
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
			bInTimeDilation = false;
		}
	}

	if (bInCombat)
	{
		RemainingResetCombatTime -= Deltatime;
		if (RemainingResetCombatTime <= 0.f)
		{
			SetInCombat(false);
		}
	}
}

void AProjectKapyCharacter::BeginDestroy()
{
	Super::BeginDestroy();
}

void AProjectKapyCharacter::Landed(const FHitResult& _hit)
{
	if (AbilityManager)
	{
		UJumpAbility* Jump = AbilityManager->GetAbilityByClass<UJumpAbility>();
		if (Jump)
		{
			Jump->Landed(_hit);
		}
	}

	if (CatCharacterMovement)
	{
		CatCharacterMovement->doubleJumpCounter = 0;

		GetOwner()->GetWorldTimerManager().SetTimer(timerClimb, this, &AProjectKapyCharacter::ResetClimb, 0.2f, false);
	}
}

void AProjectKapyCharacter::ResetClimb()
{
	CatCharacterMovement->isAbleToClimb = true;
}

void AProjectKapyCharacter::OnMovementModeCustomChanged(EmovementModeCustom CustomMovement)
{
	if (CustomMovement == EmovementModeCustom::CUSTOM_Climbing && CharacterState != ECharacterState::STATE_Climb)
	{
		SetCharacterState(ECharacterState::STATE_Climb);
	}
	else if (CustomMovement != EmovementModeCustom::CUSTOM_Climbing && CharacterState == ECharacterState::STATE_Climb)
	{
		SetCharacterState(ECharacterState::STATE_None);
	}
}

void AProjectKapyCharacter::SetCharacterState(ECharacterState newCharacterState)
{
	if (newCharacterState == CharacterState)
	{
		return;
	}

	if (CharacterState == ECharacterState::STATE_Dead && !LifeSystem->IsAlive())
	{
		return;
	}

	if (newCharacterState == ECharacterState::STATE_Melee)
	{
		if (CharacterState != ECharacterState::STATE_None)
		{
			return;
		}
	}

	if (newCharacterState == ECharacterState::STATE_Parade)
	{
		if (CharacterState != ECharacterState::STATE_None)
		{
			return;
		}
	}

	if (newCharacterState == ECharacterState::STATE_Parade)
	{
		if (CharacterState != ECharacterState::STATE_None)
		{
			return;
		}
	}

	if (newCharacterState == ECharacterState::STATE_Shoot)
	{
		if (CharacterState != ECharacterState::STATE_None)
		{
			return;
		}
	}

	if (newCharacterState == ECharacterState::STATE_PushingBlock)
	{
		if (CharacterState != ECharacterState::STATE_None)
			return;
	}


	ECharacterState PreviousState = CharacterState;
	CharacterState = newCharacterState;
	OnCharacterStateChanged(PreviousState);
}

void AProjectKapyCharacter::OnCharacterStateChanged(ECharacterState previousState)
{
	//Reset Old State
	if (previousState == ECharacterState::STATE_Shoot)
	{
		isAimingMode = false;
		CatCharacterMovement->bOrientRotationToMovement = true;
	}

	if (previousState == ECharacterState::STATE_Melee)
	{
		Controller->ResetIgnoreMoveInput();
	}
	if (previousState == ECharacterState::STATE_Stun || previousState == ECharacterState::STATE_Dead)
	{
		Controller->ResetIgnoreMoveInput();
		AbilityManager->SetBlockRequest(false);
	}

	//Modify it please, it's ugly
	if (previousState == ECharacterState::STATE_Climb)
	{
		const FVector Location = GetMesh()->GetRelativeLocation();
		GetMesh()->SetRelativeLocation(FVector(Location.X - 10.f, Location.Y, Location.Z));
	}

	if (previousState == ECharacterState::STATE_FallDamage)
	{
		AbilityManager->SetBlockRequest(false);
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}

	if (previousState == STATE_PushingBlock)
	{
		const FVector Location = GetMesh()->GetRelativeLocation();
		GetMesh()->SetRelativeLocation(FVector(Location.X + 30.f, Location.Y, Location.Z));
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}

	//Init new State
	if (CharacterState == ECharacterState::STATE_None)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}

	if (CharacterState == ECharacterState::STATE_Shoot)
	{
		isAimingMode = true;
		CatCharacterMovement->bOrientRotationToMovement = false;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}

	if (CharacterState == ECharacterState::STATE_Climb)
	{
		const FVector Location = GetMesh()->GetRelativeLocation();
		GetMesh()->SetRelativeLocation(FVector(Location.X + 10.f, Location.Y, Location.Z));

		GetCharacterMovement()->MaxCustomMovementSpeed = climbSpeed;
		GetCharacterMovement()->MaxWalkSpeed = climbSpeed;

		if (AbilityManager && AbilityManager->IsAbilityExecuting())
		{
			AbilityManager->GetAbilityExecuting()->Interrupt();
		}
	}

	if (CharacterState == ECharacterState::STATE_Melee)
	{
		// GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		Controller->SetIgnoreMoveInput(true);
	}

	if (CharacterState == ECharacterState::STATE_PushingBlock)
	{
		const FVector Location = GetMesh()->GetRelativeLocation();
		GetMesh()->SetRelativeLocation(FVector(Location.X - 30.f, Location.Y, Location.Z));

		isAimingMode = false;
		CatCharacterMovement->bOrientRotationToMovement = false;
		GetCharacterMovement()->MaxWalkSpeed = PushBlockSpeed;
	}

	if (CharacterState == ECharacterState::STATE_Cast)
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}

	if (CharacterState == ECharacterState::STATE_Stun || CharacterState == ECharacterState::STATE_Dead)
	{
		Controller->SetIgnoreMoveInput(true);
		AbilityManager->SetBlockRequest(true);
	}

	if (CharacterState == ECharacterState::STATE_FallDamage)
	{
		AbilityManager->SetBlockRequest(true);
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * 0.5f;
	}

	BP_OnCharacterStateChanged(previousState);
}

UCameraComponent* AProjectKapyCharacter::GetCameraComponent()
{
	return FollowCamera;
}

void AProjectKapyCharacter::RequestTimeDilation(float TimeDilation, float Duration)
{
	if (!bInTimeDilation && IsPlayerControlled())
	{
		bInTimeDilation = true;
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);
		TimeDilationDuration = Duration * TimeDilation;
	}
}

void AProjectKapyCharacter::SetSpeedModifier(float Modifier)
{
	SpeedModifier = Modifier;
	WalkSpeed = WalkSpeedBase * SpeedModifier;
	RunSpeed = RunSpeedBase * SpeedModifier;
}

void AProjectKapyCharacter::SetInCombat(bool InCombat)
{
	if (bInCombat != InCombat)
	{
		BP_OnNewCombatState(InCombat);
	}
	bInCombat = InCombat;
	if (bInCombat)
	{
		RemainingResetCombatTime = TimeToResetCombat;
	}
}

void AProjectKapyCharacter::OnDeath()
{
	AbilityManager->InterruptAll();
	AbilityManager->SetBlockRequest(true);
	EffectReceiver->ClearEffects();
	SetCharacterState(ECharacterState::STATE_Dead);
}

void AProjectKapyCharacter::OnRevive()
{
	AbilityManager->SetBlockRequest(false);
	SetCharacterState(ECharacterState::STATE_None);
}
