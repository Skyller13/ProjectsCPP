// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum EObjectivesType
{
    Location, //Reach the given location
    Interaction, //Interact with someone or smt
    Collect, //Collect specific object in the world
    Kill, //Kill specific Enemy (Boss, ...)
    Protect, //Protect NPC or smt
    Follow, //Follow NPC or smt
    ListenConversation, //Listen dialogue (NPC/ Enemy)
    ClearArea, //From Enemies
    SearchArea, //Reveal or pick up objective
    Chase, //Chase a specific enemy/ NPC...
    Custom, //Custom Event (Bind your own event to trigger the quest and check if it's complete)
};
