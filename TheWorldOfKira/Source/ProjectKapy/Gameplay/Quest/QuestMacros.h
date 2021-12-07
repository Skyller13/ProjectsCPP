// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Quest.h"

/**
 * 
 */
class PROJECTKAPY_API QuestMacros
{
public:
	QuestMacros();
	~QuestMacros();

	//void PlayTurnIn(); 

	AQuest* CheckForInteraction(AActor* _actor);

	//DisplayQuest();

	void UpdateUIActiveQuest(UWorld* _world);

	bool CheckForCollectible(AActor* _item);
};
