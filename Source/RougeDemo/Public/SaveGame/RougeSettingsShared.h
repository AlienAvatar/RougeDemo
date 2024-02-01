// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SubtitleDisplayOptions.h"
#include "GameFramework/SaveGame.h"
#include "RougeSettingsShared.generated.h"

class URougeLocalPlayer;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API URougeSettingsShared : public USaveGame
{
	GENERATED_BODY()
	
public:
	URougeSettingsShared();

	void Initialize(URougeLocalPlayer* LocalPlayer);
	void ApplySettings();

	static URougeSettingsShared* LoadOrCreateSettings(const URougeLocalPlayer* LocalPlayer);

	UPROPERTY(Transient)
	TObjectPtr<URougeLocalPlayer> OwningPlayer = nullptr;

	void ApplySubtitleOptions();

	DECLARE_EVENT_OneParam(URougeSettingsShared, FOnSettingChangedEvent, URougeSettingsShared* Settings);
	FOnSettingChangedEvent OnSettingChanged;

	UFUNCTION()
	bool GetForceFeedbackEnabled() const { return bForceFeedbackEnabled; }
private:
	UPROPERTY()
	ESubtitleDisplayTextSize SubtitleTextSize = ESubtitleDisplayTextSize::Medium;

	UPROPERTY()
	ESubtitleDisplayTextColor SubtitleTextColor = ESubtitleDisplayTextColor::White;

	UPROPERTY()
	ESubtitleDisplayTextBorder SubtitleTextBorder = ESubtitleDisplayTextBorder::None;

	UPROPERTY()
	ESubtitleDisplayBackgroundOpacity SubtitleBackgroundOpacity = ESubtitleDisplayBackgroundOpacity::Medium;

	UPROPERTY()
	bool bForceFeedbackEnabled = true;
};
