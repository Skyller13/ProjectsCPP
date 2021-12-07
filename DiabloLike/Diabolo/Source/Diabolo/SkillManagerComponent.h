// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DiaboloCharacter.h"
#include "Components/ActorComponent.h"
#include "SkillManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIABOLO_API USkillManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillManagerComponent();

	UPROPERTY(EditAnywhere, Instanced)
	TArray<class USkillBase*> Skills;

	void OnSkillRequested(const int32 _skillIndex);

	bool IsASkillExecuting();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	class ACharacter* GetOwnerCharacter() const;

	UFUNCTION(BlueprintCallable)
	TArray<USkillBase*> GetSkills();
};
