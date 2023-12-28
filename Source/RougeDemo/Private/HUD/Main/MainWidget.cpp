// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Main/MainWidget.h"

#include "HUD/Main/ButtonComponentWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RougeDemo/RougeDemo.h"

DEFINE_LOG_CATEGORY(LogRougeWidget);

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_QuitGame->UIMainMenu = EUIMainMenu::EUI_MM_Quit;
	Btn_StartGame->UIMainMenu = EUIMainMenu::EUI_MM_Start;
	Btn_Options->UIMainMenu = EUIMainMenu::EUI_MM_Options;
}