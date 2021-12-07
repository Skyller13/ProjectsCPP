// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserAbility.h"

ULaserAbility::ULaserAbility()
{
	ActionName = "Laser";
}

void ULaserAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsBlending)
	{
		ElapsedTime += DeltaTime;
		if (ElapsedTime >= BlendTime)
		{
			ElapsedTime = BlendTime;
			bIsBlending = false;
			BlendFactor = 1.f;
			BP_OnBlendEnded();
		}
		else
		{
			BlendFactor = ElapsedTime / BlendTime;
		}
	}
}

void ULaserAbility::SetBlend(float Time)
{
	if (Time > 0.f)
	{
		bIsBlending = true;
		BlendTime = Time;
		ElapsedTime = 0.f;
		BlendFactor = 0.f;
	}
	else
	{
		bIsBlending = false;
		BlendFactor = 1.f;
	}
}
