// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Game/LevelUpCardComponentWidget.h"

#include "Components/Button.h"

void ULevelUpCardComponentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Selected->OnClicked.AddDynamic(this, &ULevelUpCardComponentWidget::Btn_SelectedCallBack);
}

void ULevelUpCardComponentWidget::SetLevelUpCardComponentWidget(FText Name, int32 Level, FText Desc, UTexture2D* Icon, EActiveAbilities AAbility,
                                                                EPassiveAbilities PAbility,EAbilityType Type)
{
	
}

void ULevelUpCardComponentWidget::Btn_SelectedCallBack()
{
	UE_LOG(LogTemp,Error,TEXT("ULevelUpCardComponentWidget::SelectedCallBack"));
}
