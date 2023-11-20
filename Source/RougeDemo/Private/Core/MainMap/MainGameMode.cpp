// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MainMap/MainGameMode.h"

#include "Core/RougeDemoInstance.h"
#include "HUD/Main/MainWidget.h"
#include "Lib/RougeDemoFunctionLibary.h"

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	if(MainWidget)
	{
		MainWidget->AddToViewport();
	}else
	{
		//MainWidget = Cast<UMainWidget>(CreateWidget(UMainWidget::StaticClass()));
	}

	/*URougeDemoInstance* GameInstance = URougeDemoFunctionLibary::GetRougeDemoGameInstance(GetWorld());
	if(GameInstance)
	{
		const FGlobalOptionsStruct GlobalOptions = GameInstance->GetGlobalOptions();
		GameInstance->SetGlobalOptions(GlobalOptions);
	}*/
}
