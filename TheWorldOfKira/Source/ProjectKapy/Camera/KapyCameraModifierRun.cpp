// Fill out your copyright notice in the Description page of Project Settings.


#include "KapyCameraModifierRun.h"


#include "GameFramework/PawnMovementComponent.h"
#include "ProjectKapy/ProjectKapyCharacter.h"

bool UKapyCameraModifierRun::ModifyCamera(float DeltaTime, FMinimalViewInfo& InOutPOV)
{
	Super::ModifyCamera(DeltaTime, InOutPOV);
	AProjectKapyCharacter* KapyCharacter = Cast<AProjectKapyCharacter>(GetViewTarget());
	if (IsValid(KapyCharacter))
	{
		float Velocity = KapyCharacter->GetVelocity().Size();

		if ( KapyCharacter->WalkSpeed >= KapyCharacter->GetMovementComponent()->GetMaxSpeed())
		{
			ApplyCameraTransition(FCameraInfo(), TransitionTime, InOutPOV, DeltaTime);
		}
		else
		{
			ApplyCameraTransition(Modifier, TransitionTime, InOutPOV, DeltaTime);
		}
	}
	return false;
}
