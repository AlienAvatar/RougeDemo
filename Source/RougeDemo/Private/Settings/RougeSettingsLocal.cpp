// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings\RougeSettingsLocal.h"

#include "CommonInputSubsystem.h"

URougeSettingsLocal::URougeSettingsLocal()
{
	if (!HasAnyFlags(RF_ClassDefaultObject) && FSlateApplication::IsInitialized())
	{
		OnApplicationActivationStateChangedHandle = FSlateApplication::Get().OnApplicationActivationStateChanged().AddUObject(this, &ThisClass::OnAppActivationStateChanged);
	}
	
	SetToDefaults();
}

URougeSettingsLocal* URougeSettingsLocal::Get()
{
	return GEngine ? CastChecked<URougeSettingsLocal>(GEngine->GetGameUserSettings()) : nullptr;
}

void URougeSettingsLocal::OnExperienceLoaded()
{
	ReapplyThingsDueToPossibleDeviceProfileChange();
}

void URougeSettingsLocal::SetToDefaults()
{
	Super::SetToDefaults();

	bUseHeadphoneMode = false;
	bUseHDRAudioMode = false;
	bSoundControlBusMixLoaded = false;

	
}

void URougeSettingsLocal::ApplyNonResolutionSettings()
{
	Super::ApplyNonResolutionSettings();

	// Check if Control Bus Mix references have been loaded,
	// Might be false if applying non resolution settings without touching any of the setters from UI
	if (!bSoundControlBusMixLoaded)
	{
		LoadUserControlBusMix();
	}

	if (UCommonInputSubsystem* InputSubsystem = UCommonInputSubsystem::Get(GetTypedOuter<ULocalPlayer>()))
	{
		InputSubsystem->SetGamepadInputType(ControllerPlatform);
	}

	
}

void URougeSettingsLocal::UpdateEffectiveFrameRateLimit()
{
	if (!IsRunningDedicatedServer())
	{
		SetFrameRateLimitCVar(GetEffectiveFrameRateLimit());
	}
}

void URougeSettingsLocal::OnAppActivationStateChanged(bool bIsActive)
{
	UpdateEffectiveFrameRateLimit();
}

void URougeSettingsLocal::ReapplyThingsDueToPossibleDeviceProfileChange()
{
	ApplyNonResolutionSettings();
}

void URougeSettingsLocal::LoadUserControlBusMix()
{
	if (GEngine)
	{
		if (const UWorld* World = GEngine->GetCurrentPlayWorld())
		{
			// if (const URougeAudioSettings* RougeAudioSettings = GetDefault<URougeAudioSettings>())
			// {
			// 	USoundControlBus* OverallControlBus = nullptr;
			// 	USoundControlBus* MusicControlBus = nullptr;
			// 	USoundControlBus* SoundFXControlBus = nullptr;
			// 	USoundControlBus* DialogueControlBus = nullptr;
			// 	USoundControlBus* VoiceChatControlBus = nullptr;
			// 	
			// }
		}
	}
}

