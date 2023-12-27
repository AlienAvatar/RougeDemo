// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enum/EActiveAbilities.h"
#include "Enum/EPassiveAbilities.h"
#include "Enum/EAbilityType.h"
#include "LevelUpCardComponentWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API ULevelUpCardComponentWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	void SetLevelUpCardComponentWidget(FText Name, int32 Level, FText Desc, UTexture2D* Icon, EActiveAbilities AAbility,
	EPassiveAbilities PAbility, EAbilityType Type);

	UPROPERTY(meta=(BindWidget))
	UButton* Btn_Selected;

private:
	void Btn_SelectedCallBack();
};
