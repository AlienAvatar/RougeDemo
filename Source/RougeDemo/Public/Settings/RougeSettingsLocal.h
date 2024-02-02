// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "RougeSettingsLocal.generated.h"

class URougeLocalPlayer;
class UPlayerMappableInputConfig;
struct FLoadedMappableConfigPair;
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
	
	void AddOrUpdateCustomKeyboardBindings(const FName MappingName, const FKey NewKey, URougeLocalPlayer* LocalPlayer);

	// 获取具有特定名称的输入配置。如果配置不存在，则返回nullptr
	UFUNCTION(BlueprintCallable)
	const UPlayerMappableInputConfig* GetInputConfigByName(FName ConfigName) const;

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

	UPROPERTY(Config)
	FName InputConfigName = TEXT("Default");

	//当前注册的输入配置数组。这是由游戏功能插件填充的
	UPROPERTY(VisibleAnywhere)
	TArray<FLoadedMappableConfigPair> RegisteredInputConfigs;
};

