// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Main/MainHUD.h"

#include "Core/RougeDemoInstance.h"
#include "HUD/Main/MainWidget.h"
#include "Lib/RougeDemoFunctionLibary.h"

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	OnBegin();
}

void AMainHUD::OnBegin()
{
	if(!MainWidgetClass){ return; }

	if(MainWidget)
	{
		MainWidget->AddToViewport();
	}else
	{
		MainWidget = CreateWidget<UMainWidget>(GetWorld(), MainWidgetClass);
		MainWidget->AddToViewport();
	}
	
	URougeDemoInstance* RougeGameInstance = URougeDemoFunctionLibary::GetRougeDemoGameInstance(GetWorld());
	if(RougeGameInstance)
	{
		const FGlobalOptionsStruct GlobalOptions = RougeGameInstance->GetGlobalOptions();
		RougeGameInstance->SetGlobalOptions(GlobalOptions);
	}
}
