// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/RougeInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Settings/RougeSettingsLocal.h"
#include "Core/RougeLocalPlayer.h"


URougeInputComponent::URougeInputComponent(const FObjectInitializer& ObjectInitializer)
{
}

// Called when the game starts
void URougeInputComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URougeInputComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URougeInputComponent::AddInputMappings(const URougeInputConfig* InputConfig,
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	URougeLocalPlayer* LocalPlayer = InputSubsystem->GetLocalPlayer<URougeLocalPlayer>();
	check(LocalPlayer);

	if (URougeSettingsLocal* LocalSettings = URougeSettingsLocal::Get())
	{
		// Tell enhanced input about any custom keymappings that the player may have customized
		for (const TPair<FName, FKey>& Pair : LocalSettings->GetCustomPlayerInputConfig())
		{
			if (Pair.Key != NAME_None && Pair.Value.IsValid())
			{
				InputSubsystem->AddPlayerMappedKey(Pair.Key, Pair.Value);
			}
		}
	}
}

