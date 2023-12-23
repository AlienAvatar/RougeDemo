// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enum/EUIState.h"
#include "Enum/UI/EUIMainMenu.h"
#include "ButtonComponentWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FTest, EUIState, UIState);

class UButton;
class URougeDemoInstance;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API UButtonComponentWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	UButton* Btn_Element;

	UFUNCTION()
	void OnElementButtonClicked();

	UPROPERTY(EditDefaultsOnly)
	EUIMainMenu UIMainMenu = EUIMainMenu::EUI_MM_None;
private:
	void OptionsWidgetHandle();

	void StartWidgetHandle();
	
};
