// Fill out your copyright notice in the Description page of Project Settings.


#include "AINavLinkProxy.h"

#include "NavLinkCustomComponent.h"


void AAINavLinkProxy::SetSmartLinkData(const FVector& RelativeStart, const FVector& RelativeEnd,
	ENavLinkDirection::Type Direction)
{
	GetSmartLinkComp()->SetLinkData(RelativeStart, RelativeEnd, Direction);
}
