// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Main/MainWidget.h"

#include "HUD/Main/ButtonComponentWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RougeDemo/RougeDemo.h"

DEFINE_LOG_CATEGORY(Widget);

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	/*Btn_StartGame->SetUserFocus(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	//绑定按键
	Btn_StartGame->OnClicked.AddDynamic(this, &UMainWidget::OnStartButtonClicked);
	Btn_Options->OnClicked.AddDynamic(this, &UMainWidget::OnOptionsButtonClicked);*/
	//Btn_QuitGame->OnElementButtonClicked().AddDynamic(this, &UMainWidget::OnQuitButtonClicked);

	Btn_QuitGame->UIMainMenu = EUIMainMenu::EUI_MM_Quit;
	Btn_StartGame->UIMainMenu = EUIMainMenu::EUI_MM_Start;
	Btn_Options->UIMainMenu = EUIMainMenu::EUI_MM_Options;
}