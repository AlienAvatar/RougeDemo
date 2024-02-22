// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/RougeTaggedActor.h"


ARougeTaggedActor::ARougeTaggedActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ARougeTaggedActor::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(StaticGameplayTags);
}

#if WITH_EDITOR
bool ARougeTaggedActor::CanEditChange(const FProperty* InProperty) const
{
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(AActor, Tags))
	{
		return false;
	}
	
	return Super::CanEditChange(InProperty);
}
#endif
