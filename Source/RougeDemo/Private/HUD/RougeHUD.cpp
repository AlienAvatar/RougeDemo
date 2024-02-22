// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/RougeHUD.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "..\..\Public\Core\RougePlayerController.h"
#include "Components/GameFrameworkComponentManager.h"
#include "HUD/PlayerOverlayWidget.h"

void ARougeHUD::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void ARougeHUD::BeginPlay()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
	Super::BeginPlay();
}

void ARougeHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	Super::EndPlay(EndPlayReason);
}

void ARougeHUD::AddPlayerOverlay(TSubclassOf<UUserWidget> PlayerOverlayClass)
{
	if(PlayerOverlayClass)
	{
		UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
		PlayerOverlayWidget = CreateWidget<UPlayerOverlayWidget>(GetOwningPlayerController(),PlayerOverlayClass);	
		PlayerOverlayWidget->AddToViewport();
	}
}

