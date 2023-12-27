// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Game/LevelMasterWidget.h"

#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/WidgetSwitcher.h"
#include "HUD/Game/LevelUpCardComponentWidget.h"
#include "HUD/Game/LevelUpItemsComponentWidget.h"

void ULevelMasterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	Btn_Intro->OnClicked.AddDynamic(this, &ULevelMasterWidget::Btn_IntroClickedCallBack);
}

void ULevelMasterWidget::ResetUI()
{
	WS_LevelUp->SetActiveWidgetIndex(0);
}

void ULevelMasterWidget::AddSelection(FText Name, int32 Level, FText Desc, UTexture2D* Icon, EActiveAbilities AAbility,
	EPassiveAbilities PAbility, EAbilityType Type)
{
	ULevelUpCardComponentWidget* LevelUpCardComponentWidget = CreateWidget<ULevelUpCardComponentWidget>(GetOwningPlayer(), ULevelUpCardComponentWidget::StaticClass());
	LevelUpCardComponentWidget->SetLevelUpCardComponentWidget(
		Name,
		Level,
		Desc,
		Icon,
		AAbility,
		PAbility,
		Type
	);
	WBP_LevelUpItems->VB_Items->AddChildToVerticalBox(LevelUpCardComponentWidget);
	
}

void ULevelMasterWidget::ResetLevelUpItems()
{
	WBP_LevelUpItems->VB_Items->ClearChildren();

	
}

void ULevelMasterWidget::Btn_IntroClickedCallBack()
{
	UE_LOG(LogTemp, Warning, TEXT("Btn_IntroCallBack"));
	WS_LevelUp->SetActiveWidgetIndex(1);
}


