// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings\RougeSettingsLocal.h"

URougeSettingsLocal* URougeSettingsLocal::Get()
{
	return GEngine ? CastChecked<URougeSettingsLocal>(GEngine->GetGameUserSettings()) : nullptr;
}

