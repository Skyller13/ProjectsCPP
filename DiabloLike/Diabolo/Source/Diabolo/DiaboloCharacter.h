// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"



#include "Damageable.h"
#include "PlayerStateEnum.h"
#include "Editor/PropertyEditorTestObject.h"
#include "GameFramework/Character.h"
#include "DiaboloCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDeath);

UCLASS(Blueprintable)
class ADiaboloCharacter : public ACharacter, public IDamageable
{
	GENERATED_BODY()

public:
	ADiaboloCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

protected:
	
	void OnSkill1Input();
	void OnSkill2Input();
	void OnSkill3Input();
	void OnSkill4Input();
	void OnLeftMouseButtonClick();
	void OnRightMouseButtonClick();

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

	UPROPERTY(EditAnywhere)
	class USkillManagerComponent* SkillMgrComp;

	UPROPERTY(EditAnywhere)
	class USkillBase* CurrentSkill;

	void OrientCharacterToMouse();
	void OrientCharacterToMouseWhileCrouch();

	void OnMoveForwardAxis(const float _value);
	void OnMoveRightAxis(const float _value);

	void Crouch();

	void Sprint();
	void Walk();

	void ResetMovementUncrouch();

	FVector currentForwardDirection = FVector::ForwardVector;
	FVector currentRightDirection = FVector::RightVector;
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FOnPlayerDeath OnPlayerDeath;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<PlayerStateEnum> currentPlayerState = Alive;
	
	UPROPERTY(BlueprintReadOnly)
	bool crouchTurnRight = false;

	UPROPERTY(BlueprintReadOnly)
	bool crouchTurnLeft = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsNowStanding = true;
	
	UPROPERTY(BlueprintReadOnly)
	bool isRunning = false;

	UPROPERTY(BlueprintReadOnly)
	bool isDashing = false;

	UPROPERTY(BlueprintReadWrite)
	bool isReloading = false;

	UPROPERTY()
	float maxWalkSpeed = 600.0f;

	UPROPERTY()
	FRotator targetRot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MoveSpeed = 10000;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MovementVectorThisFrame = FVector::ZeroVector;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class ULevellingSystem* levelSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class UWeaponComponent* WeaponComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* Sk_Weapon; //Weapon visual skeletal mesh
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AActor*> FadedActors;

	void OnSkillRequested(const int32 _skillIndex);

	UFUNCTION(BlueprintCallable)
	void SetCurrentSkill(USkillBase* _skill);

	UFUNCTION(BlueprintCallable)
	USkillBase* GetCurrentSkill();

	UFUNCTION(BlueprintCallable)
	void RotateCameraRight();
 
	UFUNCTION(BlueprintCallable)
    void RotateCameraLeft();

	UFUNCTION(BlueprintCallable)
	void ReloadCurrentWeapon();

	UFUNCTION(BlueprintCallable)
	void EndReloadCurrentWeapon();

	UFUNCTION(BlueprintImplementableEvent)
	void Event_WeaponReload();

	UFUNCTION(BlueprintCallable)
	void SwapWeapon(float _Value);

	UFUNCTION(BlueprintCallable)
	void EquipUnequipWeapon();

	UFUNCTION(BlueprintImplementableEvent)
	void EquipCurrentWeapon();

	UFUNCTION(BlueprintImplementableEvent)
	void UnequipCurrentWeapon();
};

