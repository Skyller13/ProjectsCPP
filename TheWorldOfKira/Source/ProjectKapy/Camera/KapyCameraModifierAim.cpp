// Fill out your copyright notice in the Description page of Project Settings.


#include "KapyCameraModifierAim.h"

#include "ProjectKapy/ProjectKapyCharacter.h"

bool UKapyCameraModifierAim::ModifyCamera(float DeltaTime, FMinimalViewInfo& InOutPOV)
{
	Super::ModifyCamera(DeltaTime, InOutPOV);

	AProjectKapyCharacter* KapyCharacter = Cast<AProjectKapyCharacter>(GetViewTarget());
	if (IsValid(KapyCharacter))
	{
		if (KapyCharacter->isAimingMode)
		{
			ApplyCameraTransition(Modifiers, 0.5, InOutPOV, DeltaTime);
			return true;
		}
		else
		{
			ApplyCameraTransition(FCameraInfo(), 0.5, InOutPOV, DeltaTime);
		}
	}


	
	return  false;
}
