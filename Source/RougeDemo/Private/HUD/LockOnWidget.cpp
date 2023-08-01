// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/LockOnWidget.h"

#include "Components/Overlay.h"

void ULockOnWidget::ToggleMarket(AActor* LockOnTarget)
{
	LockOnTargetRef = LockOnTarget;
	if(LockOnTargetRef)
	{
		OV_LOCK->SetVisibility(ESlateVisibility::Visible);
		//To do Play Combat Music
	}else
	{
		OV_LOCK->SetVisibility(ESlateVisibility::Collapsed);
	}
}
