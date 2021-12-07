// Fill out your copyright notice in the Description page of Project Settings.


#include "KapyCameraModifierInCombat.h"

#include "ProjectKapy/ProjectKapyCharacter.h"

bool UKapyCameraModifierInCombat::ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV)
{
	Super::ModifyCamera(DeltaTime, InOutPOV);

	AProjectKapyCharacter* KapyCharacter = Cast<AProjectKapyCharacter>(GetViewTarget());
	if (IsValid(KapyCharacter))
	{
		RemainingTimeNoCombat -= DeltaTime;
		if (KapyCharacter->bInCombat)
		{
			RemainingTimeNoCombat = TimeToReturnNormal;
		}

		if (KapyCharacter->isAimingMode)
		{
			return false;
		}

		if (RemainingTimeNoCombat > 0.f)
		{
			ApplyCameraTransition(Modifiers, CombatTransitionTime, InOutPOV, DeltaTime);
		}
		else
		{
			ApplyCameraTransition(FCameraInfo(), ReturnNormalTransitionTime, InOutPOV, DeltaTime);
			RemainingTimeNoCombat = 0.f;
		}
	}
	return false;
}
