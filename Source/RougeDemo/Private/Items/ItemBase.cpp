// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemBase.h"

bool UItemBase::IsConsumable() const
{
	if (MaxCount <= 0)
	{
		return true;
	}
	return false;
}

FString UItemBase::GetIdentifierString() const
{
	return GetPrimaryAssetId().ToString();
}


FPrimaryAssetId UItemBase::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(ItemType, GetFName());
}
