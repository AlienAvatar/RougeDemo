// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerOverlayWidget.h"

#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "HUD/Game/AbilityTitleWidget.h"
#include "Lib/RougeDemoFunctionLibary.h"
#include "RougeDemo/RougeDemo.h"

void UPlayerOverlayWidget::UpdateTime(FText Time)
{
	TB_Time->SetText(Time);
}

void UPlayerOverlayWidget::BuildHotbar(TMap<EActiveAbilities, int32> ActiveMap,
	TMap<EPassiveAbilities, int32> PassiveMap)
{
	if(AbilityTitleWidgetClass == nullptr)
	{
		WIDGET_LOG(Error, TEXT("Can't found AbilityTitleWidgetClass, maybe not setting"))
		return;
	}
	HB_Active->ClearChildren();
	TArray<EActiveAbilities> AKeysArr;
	ActiveMap.GetKeys(AKeysArr);
	
	for(auto AKey : AKeysArr)
	{
		int32 Level = *ActiveMap.Find(AKey);
		UTexture2D* Texture = URougeDemoFunctionLibary::FindActionIcon(AKey);
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetOwningPlayer(), AbilityTitleWidgetClass);
		UAbilityTitleWidget* AbilityTitleWidget = Cast<UAbilityTitleWidget>(Widget);
		if(AbilityTitleWidget)
		{
			AbilityTitleWidget->SetUp(Level, Texture);
			HB_Active->AddChild(AbilityTitleWidget);
		}
	}

	HB_Passive->ClearChildren();
	TArray<EPassiveAbilities> PKeysArr;
	PassiveMap.GetKeys(PKeysArr);
	for(auto PKey : PKeysArr)
	{
		int32 Level = *PassiveMap.Find(PKey);
		UTexture2D* Texture = URougeDemoFunctionLibary::FindPassiveIcon(PKey);
		UAbilityTitleWidget* Widget = CreateWidget<UAbilityTitleWidget>(GetOwningPlayer(), AbilityTitleWidgetClass);
		UAbilityTitleWidget* AbilityTitleWidget = Cast<UAbilityTitleWidget>(Widget);
		if(AbilityTitleWidget)
		{
			AbilityTitleWidget->SetUp(Level, Texture);
			HB_Passive->AddChild(AbilityTitleWidget);
		}
	}
}
