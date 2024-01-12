// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/RougeHUD.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "..\..\Public\Core\RougePlayerController.h"
#include "HUD/PlayerOverlayWidget.h"

void ARougeHUD::BeginPlay()
{
	Super::BeginPlay();

	AddPlayerOverlay();
}

void ARougeHUD::AddPlayerOverlay()
{
	ARougePlayerController* PlayerController = Cast<ARougePlayerController>(GetOwningPlayerController());
	if(PlayerController && PlayerOverlayClass)
	{
		UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
		PlayerOverlayWidget = CreateWidget<UPlayerOverlayWidget>(PlayerController,PlayerOverlayClass);	
		PlayerOverlayWidget->AddToViewport();

		PlayerController->UpdateHotbar();

	}

	
}

