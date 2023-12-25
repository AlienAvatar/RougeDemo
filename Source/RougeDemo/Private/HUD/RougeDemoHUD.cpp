// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/RougeDemoHUD.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "HUD/PlayerOverlayWidget.h"

void ARougeDemoHUD::BeginPlay()
{
	Super::BeginPlay();

	AddPlayerOverlay();
}

void ARougeDemoHUD::AddPlayerOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if(PlayerController && PlayerOverlayClass)
	{
		UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
		PlayerOverlayWidget = CreateWidget<UPlayerOverlayWidget>(PlayerController,PlayerOverlayClass);	
		PlayerOverlayWidget->AddToViewport();
	}
}

