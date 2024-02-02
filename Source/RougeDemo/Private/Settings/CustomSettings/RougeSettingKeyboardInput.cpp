// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/CustomSettings/RougeSettingKeyboardInput.h"

#include "Core/RougeLocalPlayer.h"
#include "Settings/RougeSettingsLocal.h"

#define LOCTEXT_NAMESPACE "RougeSettings"

URougeSettingKeyboardInput::URougeSettingKeyboardInput()
{
	bReportAnalytics = false;
}

bool URougeSettingKeyboardInput::ChangeBinding(int32 InKeyBindSlot, FKey NewKey)
{
	// Early out if they hit the same button that is already bound. This allows for them to exit binding if they made a mistake.
	if ((InKeyBindSlot == 0 && FirstMappableOption.InputMapping.Key == NewKey) || (InKeyBindSlot == 1 && SecondaryMappableOption.InputMapping.Key == NewKey))
	{
		return false;
	}

	if (!NewKey.IsGamepadKey())
	{
		URougeLocalPlayer* RougeLocalPlayer = CastChecked<URougeLocalPlayer>(LocalPlayer);
		URougeSettingsLocal* LocalSettings = RougeLocalPlayer->GetLocalSettings();
		if (InKeyBindSlot == 0)
		{
			LocalSettings->AddOrUpdateCustomKeyboardBindings(FirstMappableOption.InputMapping.PlayerMappableOptions.Name, NewKey, RougeLocalPlayer);
			FirstMappableOption.InputMapping.Key = NewKey;
		}
		else if (InKeyBindSlot == 1)
		{
			// If there is no default secondary binding then we can create one based off of data from the primary binding
			if (SecondaryMappableOption.InputMapping.PlayerMappableOptions.Name == NAME_None)
			{
				SecondaryMappableOption = FKeyboardOption(FirstMappableOption);
			}
			
			LocalSettings->AddOrUpdateCustomKeyboardBindings(SecondaryMappableOption.InputMapping.PlayerMappableOptions.Name, NewKey, RougeLocalPlayer);
			SecondaryMappableOption.InputMapping.Key = NewKey;
		}
		else
		{
			ensureMsgf(false, TEXT("Invalid key bind slot provided!"));
		}

		//当键位没有绑定某个键时或无初始值时
		NotifySettingChanged(EGameSettingChangeReason::Change);

		return true;
	}

	return false;
}

void URougeSettingKeyboardInput::StoreInitial()
{
	FirstMappableOption.SetInitialValue(FirstMappableOption.InputMapping.Key);
	SecondaryMappableOption.SetInitialValue(SecondaryMappableOption.InputMapping.Key);
}

void URougeSettingKeyboardInput::RestoreToInitial()
{
	ChangeBinding(0, FirstMappableOption.GetInitialStoredValue());
	ChangeBinding(1, SecondaryMappableOption.GetInitialStoredValue());
}

void URougeSettingKeyboardInput::OnInitialized()
{
	DynamicDetails = FGetGameSettingsDetails::CreateLambda([this](ULocalPlayer&) {
		return FText::Format(LOCTEXT("DynamicDetails_KeyboardInputAction", "Bindings for {0}"), FirstMappableOption.InputMapping.PlayerMappableOptions.DisplayName);
	});

	Super::OnInitialized();
}

void FKeyboardOption::SetInitialValue(FKey InKey)
{
	InitialMapping = InKey;
}
