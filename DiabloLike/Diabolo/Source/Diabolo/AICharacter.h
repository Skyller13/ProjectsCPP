// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Damageable.h"
#include "PlayerStateEnum.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathCharacter);

UCLASS()
class DIABOLO_API AAICharacter : public ACharacter, public IDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = "Skills")
	class USkillBase* CurrentSkill;

	UPROPERTY(EditAnywhere, Category = "Fade")
	UTexture* textureCharacterDefault;

	UPROPERTY(EditAnywhere, Category = "Fade")
	UTexture* textureWeaponDefault;
	
	float timerFadeOut;

	UPROPERTY(EditAnywhere, Category = "Fade")
	float remainingTimeFadeOut;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade")
	UMaterialInterface* fadeOutMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	FOnDeathCharacter OnDeathCharacter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	TEnumAsByte<PlayerStateEnum> state = Alive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* LifeWidgetComponent;

	UPROPERTY(EditAnywhere)
	class USkillManagerComponent* SkillManagerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UUserWidget* HealthWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	int experienceAmount = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	int currentLevel = 1;

	UFUNCTION()
	void ChasePlayerOnDamageReceive();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Fade the actor when he die and destroy it at the end of the remaining time
	virtual void FadeActorDeath(float _DeltaTime);

	UFUNCTION(BlueprintCallable)
	void SetCurrentSkill(USkillBase* _skill);

	UFUNCTION(BlueprintCallable)
	USkillBase* GetCurrentSkill();

	UFUNCTION(BlueprintCallable)
	virtual void GiveExperienceToPlayer();
};
