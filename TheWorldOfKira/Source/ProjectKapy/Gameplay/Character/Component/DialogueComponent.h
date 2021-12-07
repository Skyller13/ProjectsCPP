// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Sound/DialogueVoice.h"


#include "DialogueComponent.generated.h"

USTRUCT(BlueprintType)
struct FDialogueOverhaulStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UDialogueWave*> Waves;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueStartSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTKAPY_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDialogueComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDialogueVoice* Voice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, FDialogueOverhaulStruct> DialogueOverhaulStructs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
	FOnDialogueStartSignature OnDialogueStartEvent;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void TalkToTarget();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isInDialogue = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Target = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* DialogueTable;
};