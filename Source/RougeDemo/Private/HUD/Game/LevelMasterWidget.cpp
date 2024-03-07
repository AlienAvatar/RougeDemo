// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Game/LevelMasterWidget.h"

#include "GameplayTagContainer.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/WidgetSwitcher.h"
#include "HUD/Game/LevelUpCardComponentWidget.h"
#include "HUD/Game/LevelUpItemsComponentWidget.h"

void ULevelMasterWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void ULevelMasterWidget::AddSelection(FText Name, FText Level, FText Desc, UTexture2D* Icon, FText Type, FGameplayTag GameplayTag)
{
	ULevelUpCardComponentWidget* LevelUpCardComponentWidget = CreateWidget<ULevelUpCardComponentWidget>(GetOwningPlayer(), LevelUpCardComponentClass);
	LevelUpCardComponentWidget->SetLevelUpCardComponentWidget(
		Name,
		Level,
		Desc,
		Icon,
		Type,
		GameplayTag
	);
	
	WBP_LevelUpItems->VB_Items->AddChildToVerticalBox(LevelUpCardComponentWidget);
	
	//Bind on Selected
	LevelUpCardComponentWidget->OnSelectedDelegate.BindUObject(this, &ULevelMasterWidget::OnSelectedDelegateEventFunction);
}

void ULevelMasterWidget::ResetLevelUpItems()
{
	WBP_LevelUpItems->VB_Items->ClearChildren();
	//Call On Ready
	OnReadyDelegate.Execute();
}

void ULevelMasterWidget::Close()
{
	WBP_LevelUpItems->VB_Items->ClearChildren();
	RemoveFromParent();
}

void ULevelMasterWidget::OnSelectedDelegateEventFunction(FGameplayTag GameplayTag)
{
	//TestLog
	//UE_LOG(LogTemp, Warning, TEXT("GameplayTag[%s]"), *GameplayTag.ToString())

	OnCloseDelegate.Execute(GameplayTag);
	// Call OnClose()
	Close();
}


