// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


#include "EnumCharacterType.h"
#include "EnumCustomMovement.h"
#include "GameFramework/Character.h"

#include "ProjectKapyCharacter.generated.h"

class UCatCharacterMovement;
class UEffectReceiverComponent;


UENUM(BlueprintType)
enum ECharacterState
{
	STATE_None = 0 UMETA(DisplayName = "None"),
	STATE_Melee = 1 UMETA(DisplayName = "Melee"),
	STATE_Shoot = 2 UMETA(DisplayName = "Shoot"),
	STATE_Parade = 3 UMETA(DisplayName = "Parade"),
	STATE_Stun = 4 UMETA(DisplayName = "Stun"),
	STATE_Dead = 5 UMETA(DisplayName = "Dead"),
	STATE_PushingBlock = 6 UMETA(DisplayName = "PushingBlock"),
	STATE_Cast = 7 UMETA(DisplayName = "Cast"),
	STATE_Interact = 8 UMETA(DisplayName = "Interact"),
	STATE_Climb = 9 UMETA(DisplayName = "Climb"),
	STATE_FallDamage = 10 UMETA(DisplayName = "FallDamage"),
	STATE_CampFire = 11 UMETA(DisplayName = "CampFire"),
	STATE_Max = 12 UMETA(DisplayName = "MAX")
};

UCLASS(config=Game)
class AProjectKapyCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UKapySpringArmComponent* CameraSpringArm;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	float WalkSpeedBase;
	float RunSpeedBase;

public:
	AProjectKapyCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCatCharacterMovement* CatCharacterMovement;

	UPROPERTY(EditAnywhere)
	FTimerHandle timerClimb;

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class ULifeSystemComponent* LifeSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UManaSystemComponent* ManaSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UAbilityManagerComponent* AbilityManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UEffectReceiverComponent* EffectReceiver;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UToonShaderPointLightDetector* ShaderComponent;

	//Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpeedModifier = 1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool isAimingMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spying")
	float RangeToHearConversation = 1000.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TEnumAsByte<enum ECharacterState> CharacterState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PushBlockSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float climbSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class APushableBlock* linkedBlock = nullptr;

	bool bInTimeDilation;

	float TimeDilationDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bInCombat;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	float RemainingResetCombatTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeToResetCombat = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ECharacterType> CharacterType;

private:


protected:

	// /** Called for forwards/backward input */
	// void MoveForward(float Value);
	//
	// /** Called for side to side input */
	// void MoveRight(float Value);
	//
	// /** 
	//  * Called via input to turn at a given rate. 
	//  * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	//  */
	// void TurnAtRate(float Rate);
	//
	// /**
	//  * Called via input to turn look up/down at a given rate. 
	//  * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	//  */
	// void LookUpAtRate(float Rate);
	//
	// UFUNCTION(BlueprintCallable)
	// void AimPressed();
	//
	// UFUNCTION(BlueprintCallable)
	// void AimRelease();
	//
	// // APawn interface
	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void Landed(const FHitResult& _hit) override;

	void ResetClimb();

	UFUNCTION()
	virtual void OnMovementModeCustomChanged(EmovementModeCustom CustomMovement);

	// void AutoSelectQuest();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class UKapySpringArmComponent* GetSpringArm() const { return CameraSpringArm; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual void BeginPlay() override;

	virtual void Tick(float Deltatime) override;

	virtual void BeginDestroy() override;


	UFUNCTION(BlueprintCallable)
	void SetCharacterState(ECharacterState newCharacterState);

	UFUNCTION()
	void OnCharacterStateChanged(ECharacterState previousState);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnCharacterStateChanged(ECharacterState previousState);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class ULifeSystemComponent* GetLifeSystem() const { return LifeSystem; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UEffectReceiverComponent* GetEffectReceiver() const { return EffectReceiver; }

	UFUNCTION(BlueprintCallable)
	UCameraComponent* GetCameraComponent();

	UFUNCTION(BlueprintCallable)
	void RequestTimeDilation(float TimeDilation, float Duration);

	UFUNCTION(BlueprintCallable)
	void SetSpeedModifier(float Modifier);


	
	UFUNCTION(BlueprintCallable)
	void SetInCombat(bool InCombat);

	UFUNCTION(BlueprintImplementableEvent)
        void BP_OnNewCombatState(bool InCombat);
	
	UFUNCTION()
	void OnDeath();
	UFUNCTION()
	void OnRevive();

	UFUNCTION(BlueprintImplementableEvent)
	void OnFocus();
	UFUNCTION(BlueprintImplementableEvent)
	void OnDefocus();

	// UFUNCTION(BlueprintCallable)
	// TMap<int32, int32> GetItemsCollected();
	//
	// UFUNCTION(BlueprintCallable)
	// void AddItemToCollection(int32 _index, int32 _value);
};
