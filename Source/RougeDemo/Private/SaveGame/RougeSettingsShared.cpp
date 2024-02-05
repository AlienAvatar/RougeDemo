// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGame/RougeSettingsShared.h"

#include "Core/RougeLocalPlayer.h"
#include "SubtitleDisplaySubsystem.h"
#include "Kismet/GameplayStatics.h"

static FString SHARED_SETTINGS_SLOT_NAME = TEXT("SharedGameSettings");

URougeSettingsShared::URougeSettingsShared()
{
}

void URougeSettingsShared::Initialize(URougeLocalPlayer* LocalPlayer)
{
	check(LocalPlayer);
	
	OwningPlayer = LocalPlayer;
}

void URougeSettingsShared::ApplySettings()
{
	ApplySubtitleOptions();
	//ApplyBackgroundAudioSettings();
	//ApplyCultureSettings();
}

URougeSettingsShared* URougeSettingsShared::LoadOrCreateSettings(const URougeLocalPlayer* LocalPlayer)
{
	URougeSettingsShared* SharedSettings = nullptr;

	//是否在SaveGame存在，存在则读取，否则则创建
	if (UGameplayStatics::DoesSaveGameExist(SHARED_SETTINGS_SLOT_NAME, LocalPlayer->GetLocalPlayerIndex()))
	{
		USaveGame* Slot = UGameplayStatics::LoadGameFromSlot(SHARED_SETTINGS_SLOT_NAME, LocalPlayer->GetLocalPlayerIndex());
		SharedSettings = Cast<URougeSettingsShared>(Slot);
	}
	
	if (SharedSettings == nullptr)
	{
		SharedSettings = Cast<URougeSettingsShared>(UGameplayStatics::CreateSaveGameObject(URougeSettingsShared::StaticClass()));
	}

	SharedSettings->Initialize(const_cast<URougeLocalPlayer*>(LocalPlayer));
	SharedSettings->ApplySettings();

	return SharedSettings;
}

void URougeSettingsShared::ApplySubtitleOptions()
{
	if (USubtitleDisplaySubsystem* SubtitleSystem = USubtitleDisplaySubsystem::Get(OwningPlayer))
	{
		FSubtitleFormat SubtitleFormat;
		SubtitleFormat.SubtitleTextSize = SubtitleTextSize;
		SubtitleFormat.SubtitleTextColor = SubtitleTextColor;
		SubtitleFormat.SubtitleTextBorder = SubtitleTextBorder;
		SubtitleFormat.SubtitleBackgroundOpacity = SubtitleBackgroundOpacity;

		SubtitleSystem->SetSubtitleDisplayOptions(SubtitleFormat);
	}
}
