// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Game/LevelUpCardComponentWidget.h"

#include "GameplayTagContainer.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void ULevelUpCardComponentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Selected->OnClicked.AddDynamic(this, &ULevelUpCardComponentWidget::Btn_SelectedCallBack);
}

void ULevelUpCardComponentWidget::SetLevelUpCardComponentWidget(FText Name, FText Level, FText Desc, UTexture2D* Icon, FText Type, FGameplayTag GameplayTag)
{
	TB_Name->SetText(Name);
	TB_Desc->SetText(Desc);
	Img_Icon->SetBrushFromTexture(Icon);
	TB_Level->SetText(Level);
	CurrentGameplayTag = GameplayTag;
}

void ULevelUpCardComponentWidget::Btn_SelectedCallBack()
{
	OnSelectedDelegate.Execute(CurrentGameplayTag);
}
