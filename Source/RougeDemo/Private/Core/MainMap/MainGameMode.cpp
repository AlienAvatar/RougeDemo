// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MainMap/MainGameMode.h"

#include "..\..\..\Public\Core\RougeInstance.h"
#include "HUD/Main/MainWidget.h"
#include "Lib/RougeDemoFunctionLibary.h"

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	/*if(MainWidget)
	{
		MainWidget->AddToViewport();
	}else
	{
		MainWidget = CreateWidget<UMainWidget>(GetWorld(), MainWidgetClass);
	}
	MainWidget->AddToViewport();
	
	URougeDemoInstance* GameInstance = URougeDemoFunctionLibary::GetRougeDemoGameInstance(GetWorld());
	if(GameInstance)
	{
		const FGlobalOptionsStruct GlobalOptions = GameInstance->GetGlobalOptions();
		GameInstance->SetGlobalOptions(GlobalOptions);
	}*/
}
