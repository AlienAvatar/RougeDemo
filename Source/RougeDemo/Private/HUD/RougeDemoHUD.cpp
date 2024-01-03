// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/RougeDemoHUD.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Core/RougeDemoPlayerController.h"
#include "HUD/PlayerOverlayWidget.h"

void ARougeDemoHUD::BeginPlay()
{
	Super::BeginPlay();

	AddPlayerOverlay();
}

void ARougeDemoHUD::AddPlayerOverlay()
{
	ARougeDemoPlayerController* PlayerController = Cast<ARougeDemoPlayerController>(GetOwningPlayerController());
	if(PlayerController && PlayerOverlayClass)
	{
		UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
		PlayerOverlayWidget = CreateWidget<UPlayerOverlayWidget>(PlayerController,PlayerOverlayClass);	
		PlayerOverlayWidget->AddToViewport();

		PlayerController->UpdateHotbar();

	}

	
}

