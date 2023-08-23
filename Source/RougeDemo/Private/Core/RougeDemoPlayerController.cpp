// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/RougeDemoPlayerController.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "HUD/PlayerOverlayWidget.h"
#include "HUD/RougeDemoHUD.h"

void ARougeDemoPlayerController::BeginPlay()
{
	Super::BeginPlay();

	RougeDemoHUD = Cast<ARougeDemoHUD>(GetHUD());
}

void ARougeDemoPlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	RougeDemoHUD = RougeDemoHUD == nullptr ? Cast<ARougeDemoHUD>(GetHUD()) : RougeDemoHUD;

	bool bHUDValid = RougeDemoHUD &&
		RougeDemoHUD->PlayerOverlayWidget &&
		RougeDemoHUD->PlayerOverlayWidget->HealthBar &&
		RougeDemoHUD->PlayerOverlayWidget->HealthText;

	if(bHUDValid)
	{
		const float HealthPercent = Health / MaxHealth;
		RougeDemoHUD->PlayerOverlayWidget->HealthBar->SetPercent(HealthPercent);
		FString HealthText = FString::Printf(TEXT("%d/%d"),FMath::CeilToInt(Health),FMath::CeilToInt(MaxHealth));
		RougeDemoHUD->PlayerOverlayWidget->HealthText->SetText(FText::FromString(HealthText));
	}
	
}
