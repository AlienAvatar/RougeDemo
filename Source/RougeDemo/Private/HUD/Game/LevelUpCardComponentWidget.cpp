// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Game/LevelUpCardComponentWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void ULevelUpCardComponentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Selected->OnClicked.AddDynamic(this, &ULevelUpCardComponentWidget::Btn_SelectedCallBack);
}

void ULevelUpCardComponentWidget::SetLevelUpCardComponentWidget(FText Name, int32 Level, FText Desc, UTexture2D* Icon, EActiveAbilities AAbility,
                                                                EPassiveAbilities PAbility,EAbilityType Type)
{
	TB_Name->SetText(Name);
	TB_Desc->SetText(Desc);
	Img_Icon->SetBrushFromTexture(Icon);
	const FString LevelStr = Level == 0 ? TEXT("New") : FString::FromInt(Level);
	const FText LevelText = FText::Format(FText::FromString("Level: {0}"), FText::FromString(LevelStr));
	TB_Level->SetText(LevelText);

	mAAbility = AAbility;
	mPAbility = PAbility;
	mType = Type;

	
}

void ULevelUpCardComponentWidget::Btn_SelectedCallBack()
{
	OnSelectedDelegate.Execute();
}
