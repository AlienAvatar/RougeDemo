// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerOverlayWidget.h"

#include "GameplayTagContainer.h"
#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "HUD/Game/AbilityTitleWidget.h"
#include "Lib/RougeDemoFunctionLibary.h"
#include "RougeDemo/RougeDemo.h"

#include "RougeDemo/RougeGameplayTags.h"

void UPlayerOverlayWidget::UpdateTime(FText Time)
{
	TB_Time->SetText(Time);
}

void UPlayerOverlayWidget::BuildHotbar(TMap<FGameplayTag, FAbilityLevelUp> AbilityMap)
{
	if(AbilityTitleWidgetClass == nullptr)
	{
		WIDGET_LOG(Error, TEXT("Can't found AbilityTitleWidgetClass, maybe not setting"))
		return;
	}
	HB_Active->ClearChildren();
	HB_Passive->ClearChildren();
	
	FRougeGameplayTags GameplayTags = FRougeGameplayTags::Get();
	
	for(TPair<FGameplayTag, FAbilityLevelUp>& Ability : AbilityMap)
	{
		if(Ability.Value.bActive)
		{
			int Level = Ability.Value.Level;
			UTexture2D* Texture = Ability.Value.AbilityIcon;

			//创建技能UI
			UUserWidget* Widget = CreateWidget<UUserWidget>(GetOwningPlayer(), AbilityTitleWidgetClass);
			UAbilityTitleWidget* AbilityTitleWidget = Cast<UAbilityTitleWidget>(Widget);
			if(AbilityTitleWidget)
			{
				AbilityTitleWidget->SetUp(Level, Texture);
				if(Ability.Key.MatchesTag(GameplayTags.Ability_Type_Magic_Warrior_ActiveAbility)) 
				{
					HB_Active->AddChild(AbilityTitleWidget);
				}else if(Ability.Key.MatchesTag(GameplayTags.Ability_Type_Magic_Warrior_PassiveAbility))
				{
					HB_Passive->AddChild(AbilityTitleWidget);
				}
			}
		}
		
	}
}

void UPlayerOverlayWidget::UpdateXP(float Percent)
{
	PB_XP->SetPercent(Percent);
}


