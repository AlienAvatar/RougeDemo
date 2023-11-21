// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Main/MainWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RougeDemo/RougeDemo.h"

DEFINE_LOG_CATEGORY(Widget);

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_StartGame->SetUserFocus(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	//绑定按键
	Btn_StartGame->OnClicked.AddDynamic(this, &UMainWidget::OnStartButtonClicked);
	Btn_Options->OnClicked.AddDynamic(this, &UMainWidget::OnOptionsButtonClicked);
	Btn_QuitGame->OnClicked.AddDynamic(this, &UMainWidget::OnQuitButtonClicked);
}

void UMainWidget::OnStartButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("StartButton"));
}

void UMainWidget::OnOptionsButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Options"));
}

void UMainWidget::OnQuitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(),nullptr, EQuitPreference::Quit,false);
}
