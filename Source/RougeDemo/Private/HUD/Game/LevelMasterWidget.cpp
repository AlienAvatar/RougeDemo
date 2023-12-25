// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Game/LevelMasterWidget.h"

#include "Components/VerticalBox.h"
#include "Components/WidgetSwitcher.h"
#include "HUD/Game/LevelUpItemsComponentWidget.h"

void ULevelMasterWidget::ResetUI()
{
	WS_LevelUp->SetActiveWidget(0);
}

void ULevelMasterWidget::ResetLevelUpItems()
{
	WBP_LevelUpItems->VB_Items->ClearChildren();
}
