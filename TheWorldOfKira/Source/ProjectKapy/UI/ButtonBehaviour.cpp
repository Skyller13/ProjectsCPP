// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonBehaviour.h"

UButtonBehaviour::UButtonBehaviour()
{
}

void UButtonBehaviour::Execute()
{
	BP_Execute();
}

void UButtonBehaviour::RequestExecution()
{
	Execute();
}
