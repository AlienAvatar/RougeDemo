// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Main/ButtonComponentWidget.h"
#include "Components/Button.h"
#include "Core/RougeDemoInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Lib/RougeDemoFunctionLibary.h"
#include "RougeDemo/RougeDemo.h"

void UButtonComponentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Element->OnClicked.AddDynamic(this, &UButtonComponentWidget::OnElementButtonClicked);
}

void UButtonComponentWidget::OnElementButtonClicked()
{
	switch (UIMainMenu)
	{
		case EUIMainMenu::EUI_MM_Start:
			StartWidgetHandle();
			break;
		case EUIMainMenu::EUI_MM_Options:
			OptionsWidgetHandle();
			break;
		case EUIMainMenu::EUI_MM_Quit:
			UKismetSystemLibrary::QuitGame(GetWorld(),nullptr, EQuitPreference::Quit,false);
			break;
	}
}

void UButtonComponentWidget::OptionsWidgetHandle()
{
	UE_LOG(LogTemp, Log, TEXT("Start[%s]"), *UEnum::GetValueAsString(UIMainMenu));
}

void UButtonComponentWidget::StartWidgetHandle()
{
	//RemoveFromParent();
	URougeDemoInstance* RougeDemoInstance = URougeDemoFunctionLibary::GetRougeDemoGameInstance(GetWorld());
	if(RougeDemoInstance)
	{
		RougeDemoInstance->LoadGameLevel();
	}
	WIDGET_LOG(Warning, TEXT("StartWidgetHandle"));
}
