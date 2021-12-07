// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Accessories.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIABOLO_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<AAccessories*> tempAccessories;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<class AWeapon>> weapons; //the inventory of the owner (store every weapon)

	UPROPERTY(VisibleAnywhere)
	TArray<AWeapon*> inventoryWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AWeapon* currentWeapon; //Current equipped weapon
	
	UPROPERTY(EditAnywhere)
	bool isWeaponEquipped = true;

	UPROPERTY(EditAnywhere)
	int32 currentWeaponIndex = 0;
	
	UFUNCTION(BlueprintCallable)
	ACharacter* GetOwnerCharacter();

	UFUNCTION(BlueprintCallable)
	void EquipNewWeapon(AWeapon* _weapon);
};
