// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "RougeSettingsLocal.generated.h"

/**
 * 
 */
UCLASS()
class URougeSettingsLocal : public UGameUserSettings
{
	GENERATED_BODY()

public:
	URougeSettingsLocal();
	
	static URougeSettingsLocal* Get();

	const TMap<FName, FKey>& GetCustomPlayerInputConfig() const { return CustomKeyboardConfig; }

	void OnExperienceLoaded();

	//~UGameUserSettings interface
	virtual void SetToDefaults() override;
	virtual void ApplyNonResolutionSettings() override;
	//~End of UGameUserSettings interface

	/** Whether to use High Dynamic Range Audio mode (HDR Audio) **/
	UPROPERTY(config)
	bool bUseHDRAudioMode;
	
	DECLARE_EVENT_OneParam(URougeSettingsLocal, FAudioDeviceChanged, const FString& /*DeviceId*/);
	FAudioDeviceChanged OnAudioOutputDeviceChanged;
protected:
	void UpdateEffectiveFrameRateLimit();
	
private:
	//自定义键盘设置映射
	UPROPERTY(Config)
	TMap<FName, FKey> CustomKeyboardConfig;

	void OnAppActivationStateChanged(bool bIsActive);
	void ReapplyThingsDueToPossibleDeviceProfileChange();

	UPROPERTY(Transient)
	bool bSoundControlBusMixLoaded;

	void LoadUserControlBusMix();

	/**
	 * The name of the controller the player is using.  This is maps to the name of a UCommonInputBaseControllerData
	 * that is available on this current platform.  The gamepad data are registered per platform, you'll find them
	 * in <Platform>Game.ini files listed under +ControllerData=...
	 */
	UPROPERTY(Config)
	FName ControllerPlatform;

	UPROPERTY(config)
	bool bUseHeadphoneMode;

	FDelegateHandle OnApplicationActivationStateChangedHandle;
};

