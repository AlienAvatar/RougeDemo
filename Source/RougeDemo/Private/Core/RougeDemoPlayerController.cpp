// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/RougeDemoPlayerController.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Character/RougeDemoCharacter.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "HUD/PlayerOverlayWidget.h"
#include "HUD/RougeDemoHUD.h"
#include "HUD/Game/LevelMasterWidget.h"

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
		const FString HealthText = FString::Printf(TEXT("%d/%d"),FMath::CeilToInt(Health),FMath::CeilToInt(MaxHealth));
		RougeDemoHUD->PlayerOverlayWidget->HealthText->SetText(FText::FromString(HealthText));
	}
}

void ARougeDemoPlayerController::SetHUDScore(float Score)
{
	RougeDemoHUD = RougeDemoHUD == nullptr ? Cast<ARougeDemoHUD>(GetHUD()) : RougeDemoHUD;

	bool bHUDValid = RougeDemoHUD &&
		RougeDemoHUD->PlayerOverlayWidget &&
		RougeDemoHUD->PlayerOverlayWidget->ScoreAmount;
	if(bHUDValid)
	{
		const FString ScoreAmountText = FString::Printf(TEXT("%d"),FMath::CeilToInt(Score));
		RougeDemoHUD->PlayerOverlayWidget->ScoreAmount->SetText(FText::FromString(ScoreAmountText));
	}
}

void ARougeDemoPlayerController::SetHUDSkillValue(float SkillValue,float MaxSkillValue)
{
	RougeDemoHUD = RougeDemoHUD == nullptr ? Cast<ARougeDemoHUD>(GetHUD()) : RougeDemoHUD;

	bool bHUDValid = RougeDemoHUD &&
		RougeDemoHUD->PlayerOverlayWidget &&
		RougeDemoHUD->PlayerOverlayWidget->SkillValueBar;

	if(bHUDValid)
	{
		const float HealthPercent = SkillValue / MaxSkillValue;
		RougeDemoHUD->PlayerOverlayWidget->HealthBar->SetPercent(HealthPercent);
	}
}

void ARougeDemoPlayerController::OnLevelUp()
{
	CreateLevelUpUI();
}

void ARougeDemoPlayerController::SetupPlayer()
{
	//检查是否是本地
	if(IsLocalPlayerController())
	{
		ARougeDemoCharacter* RougeDemoCharacter = Cast<ARougeDemoCharacter>(GetPawn());
		/*AbilityComponent = RougeDemoCharacter*/
		
	}
}

void ARougeDemoPlayerController::SetReference()
{
	
}

void ARougeDemoPlayerController::CreateLevelUpUI()
{
	if(HasAuthority())
	{
		PrepareLevelUp();
	}
}

void ARougeDemoPlayerController::PrepareLevelUp()
{
	//创建LevelUpUI
	if(!LevelMasterWidget)
	{
		LevelMasterWidget = CreateWidget<ULevelMasterWidget>(this, ULevelMasterWidget::StaticClass());
		ExecuteLevelUp();
	}else
	{
		LevelMasterWidget->ResetUI();
	}
	LevelMasterWidget->AddToViewport();
	
}

void ARougeDemoPlayerController::ExecuteLevelUp()
{
	
}
