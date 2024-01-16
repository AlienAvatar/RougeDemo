// Fill out your copyright notice in the Description page of Project Settings.


#include "Assets/RougeGameData.h"

#include "Assets/RougeAssetManager.h"

URougeGameData::URougeGameData()
{
}

const URougeGameData& URougeGameData::Get()
{
	return URougeAssetManager::Get().GetGameData();
}
