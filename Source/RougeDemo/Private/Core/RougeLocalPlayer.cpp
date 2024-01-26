// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/RougeLocalPlayer.h"

#include "Settings/RougeSettingsLocal.h"

URougeLocalPlayer::URougeLocalPlayer()
{
}

void URougeLocalPlayer::PostInitProperties()
{
	Super::PostInitProperties();

	if (URougeSettingsLocal* LocalSettings = GetLocalSettings())
	{
		LocalSettings->OnAudioOutputDeviceChanged.AddUObject(this, &URougeLocalPlayer::OnAudioOutputDeviceChanged);
	}
}

void URougeLocalPlayer::SwitchController(APlayerController* PC)
{
	Super::SwitchController(PC);

	OnPlayerControllerChanged(PlayerController);
}

bool URougeLocalPlayer::SpawnPlayActor(const FString& URL, FString& OutError, UWorld* InWorld)
{
	const bool bResult = Super::SpawnPlayActor(URL, OutError, InWorld);
	OnPlayerControllerChanged(PlayerController);
	
	return bResult;
}

void URougeLocalPlayer::InitOnlineSession()
{
	OnPlayerControllerChanged(PlayerController);
	
	Super::InitOnlineSession();
}

void URougeLocalPlayer::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	IRougeTeamAgentInterface::SetGenericTeamId(NewTeamID);
}

FGenericTeamId URougeLocalPlayer::GetGenericTeamId() const
{
	return IRougeTeamAgentInterface::GetGenericTeamId();
}

FOnRougeTeamIndexChangedDelegate* URougeLocalPlayer::GetOnTeamIndexChangedDelegate()
{
	return &OnTeamChangedDelegate;
}

URougeSettingsLocal* URougeLocalPlayer::GetLocalSettings() const
{
	return URougeSettingsLocal::Get();
}

void URougeLocalPlayer::OnAudioOutputDeviceChanged(const FString& InAudioOutputDeviceId)
{
}

void URougeLocalPlayer::OnPlayerControllerChanged(APlayerController* NewController)
{
	//停止监听旧的PlayerController
	FGenericTeamId OldTeamID = FGenericTeamId::NoTeam;
	if (IRougeTeamAgentInterface* ControllerAsTeamProvider = Cast<IRougeTeamAgentInterface>(LastBoundPC.Get()))
	{
		OldTeamID = ControllerAsTeamProvider->GetGenericTeamId();
		ControllerAsTeamProvider->GetTeamChangedDelegateChecked().RemoveAll(this);
	}
	
	//监听现在的TeamID
	FGenericTeamId NewTeamID = FGenericTeamId::NoTeam;
	if (IRougeTeamAgentInterface* ControllerAsTeamProvider = Cast<IRougeTeamAgentInterface>(NewController))
	{
		NewTeamID = ControllerAsTeamProvider->GetGenericTeamId();
		ControllerAsTeamProvider->GetTeamChangedDelegateChecked().AddDynamic(this, &ThisClass::OnControllerChangedTeam);
		LastBoundPC = NewController;
	}
	
	ConditionalBroadcastTeamChanged(this, OldTeamID, NewTeamID);
}

void URougeLocalPlayer::OnControllerChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam)
{
	ConditionalBroadcastTeamChanged(this, IntegerToGenericTeamId(OldTeam), IntegerToGenericTeamId(NewTeam));
}
