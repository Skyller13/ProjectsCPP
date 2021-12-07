// Fill out your copyright notice in the Description page of Project Settings.


#include "KapyPlayerController.h"

AKapyPlayerController::AKapyPlayerController()
{
}

void AKapyPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bInBlendCamera)
	{
		TimeRemainingCameraBlend -= DeltaSeconds;
		if (TimeRemainingCameraBlend < 0.f)
		{
			TimeRemainingCameraBlend = 0.f;
			if (GetViewTarget() == GetPawn())
			{
				bInBlendCamera = false;
				ResetIgnoreLookInput();
			}
			else
			{
				SwitchCameraBlend();
			}
		}
	}
}

void AKapyPlayerController::ChangeView(FCameraBlend& CameraBlend)
{
	if (IsValid(CameraBlend.ViewTarget))
	{
		SetViewTargetWithBlend(CameraBlend.ViewTarget, CameraBlend.BlendTime, CameraBlend.BlendFunction,
		                       CameraBlend.BlendExp);
		TimeRemainingCameraBlend = CameraBlend.BlendTime + CameraBlend.StayTime;
	}
}

void AKapyPlayerController::SwitchCameraBlend()
{
	if (NextView < CameraSequence.CameraBlends.Num())
	{
		if (IsValid(CameraSequence.CameraBlends[NextView].ViewTarget))
		{
			ChangeView(CameraSequence.CameraBlends[NextView]);
			NextView = NextView+ 1;
			return;
		}
		else
		{
			CameraSequence.CameraBlends[NextView].ViewTarget = GetPawn();
			ChangeView(CameraSequence.CameraBlends[NextView]);
			NextView = NextView + 1;
			return;
		}
	}
	
	if (GetViewTarget() != GetPawn())
	{
		ReturnToPawnView();
	}
}

void AKapyPlayerController::ReturnToPawnView()
{
	ReturnToPlayerBlend.ViewTarget = GetPawn();
	ChangeView(ReturnToPlayerBlend);
}

void AKapyPlayerController::SetCameraSequence(const FCameraSequence _Sequence)
{
	if (_Sequence.CameraBlends.Num() > 0)
	{
		NextView = 0;
		bInBlendCamera = true;
		CameraSequence = _Sequence;
		SetIgnoreLookInput(true);
		SwitchCameraBlend();
	}
}
