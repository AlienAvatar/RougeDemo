// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerOverlayWidget.h"

#include "Components/TextBlock.h"

void UPlayerOverlayWidget::UpdateTime(FText Time)
{
	TB_Time->SetText(Time);
}
