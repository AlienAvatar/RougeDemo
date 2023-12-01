// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Main/MainWidget.h"

#include "Components/Button.h"
#include "Core/RougeDemoInstance.h"
#include "HUD/Main/ButtonComponentWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Lib/RougeDemoFunctionLibary.h"
#include "RougeDemo/RougeDemo.h"

DEFINE_LOG_CATEGORY(Widget);

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	/*Btn_StartGame->SetUserFocus(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	//绑定按键
	Btn_StartGame->OnClicked.AddDynamic(this, &UMainWidget::OnStartButtonClicked);
	Btn_Options->OnClicked.AddDynamic(this, &UMainWidget::OnOptionsButtonClicked);*/
	//Btn_QuitGame.AddDynamic(this, &UMainWidget::OnQuitButtonClicked);

	
}

void UMainWidget::OnStartButtonClicked()
{
	WIDGET_LOG(Log, TEXT("StartButton"));
	URougeDemoInstance* RougeDemoInstance = URougeDemoFunctionLibary::GetRougeDemoGameInstance(GetWorld());
	if(RougeDemoInstance)
	{
		RougeDemoInstance->LoadGameLevel();
	}
}

void UMainWidget::OnOptionsButtonClicked()
{
	WIDGET_LOG(Log, TEXT("Options"));
}

void UMainWidget::OnQuitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(),nullptr, EQuitPreference::Quit,false);
}
