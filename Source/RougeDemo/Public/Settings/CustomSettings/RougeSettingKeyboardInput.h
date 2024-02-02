// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedActionKeyMapping.h"
#include "GameSettingValue.h"
#include "RougeSettingKeyboardInput.generated.h"

/**
 * RougeSettingKeyboardInput
 */
class UPlayerMappableInputConfig;
struct FKeyboardOption
{
	FKeyboardOption() = default;
	
	FEnhancedActionKeyMapping InputMapping {};
	
	const UPlayerMappableInputConfig* OwningConfig = nullptr;

	void ResetToDefault();

	/** Store the currently set FKey that this is bound to */
	void SetInitialValue(FKey InKey);

	/** Get the most recently stored initial value */
	FKey GetInitialStoredValue() const { return InitialMapping; };

private:

	/** The key that this option is bound to initially, used in case the user wants to cancel their mapping */
	FKey InitialMapping;
};

/**
 * 键位绑定
 */
UCLASS()
class ROUGEDEMO_API URougeSettingKeyboardInput : public UGameSettingValue
{
	GENERATED_BODY()

public:
	URougeSettingKeyboardInput();

	bool ChangeBinding(int32 InKeyBindSlot, FKey NewKey);
	

	virtual void StoreInitial() override;
	virtual void RestoreToInitial() override;
protected:
	//第一个功能键，主键
	FKeyboardOption FirstMappableOption;
	//第二个功能键，附键
	FKeyboardOption SecondaryMappableOption;

	// UGameSettingValue Interface Begin
	virtual void OnInitialized() override;
};
