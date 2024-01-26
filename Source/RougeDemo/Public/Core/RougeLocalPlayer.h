// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonLocalPlayer.h"
#include "Teams/RougeTeamAgentInterface.h"
#include "RougeLocalPlayer.generated.h"

class URougeSettingsLocal;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API URougeLocalPlayer : public UCommonLocalPlayer, public IRougeTeamAgentInterface
{
	GENERATED_BODY()

public:

	URougeLocalPlayer();

	//~UObject interface
	//PostInitProperties的调用线程与对象的创建线程是一个线程，因些PostInitProperties也有可能在非主线程中回调
	virtual void PostInitProperties() override;
	//~End of UObject interface

	//~UPlayer interface
	virtual void SwitchController(class APlayerController* PC) override;
	//~End of UPlayer interface

	//~ULocalPlayer interface
	virtual bool SpawnPlayActor(const FString& URL, FString& OutError, UWorld* InWorld) override;
	virtual void InitOnlineSession() override;
	//~End of ULocalPlayer interface

	//~IRougeTeamAgentInterface interface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnRougeTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//~End of IRougeTeamAgentInterface interface

	UFUNCTION()
	URougeSettingsLocal* GetLocalSettings() const;

protected:
	void OnAudioOutputDeviceChanged(const FString& InAudioOutputDeviceId);
private:
	//更改PlayerController
	void OnPlayerControllerChanged(APlayerController* NewController);

	UPROPERTY()
	TWeakObjectPtr<APlayerController> LastBoundPC;

	UFUNCTION()
	void OnControllerChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);

	UPROPERTY()
	FOnRougeTeamIndexChangedDelegate OnTeamChangedDelegate;
};
