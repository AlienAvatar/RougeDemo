// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/RougeAbilitySet.h"

#include "AbilitySystem/RougeAbilitySystemComponent.h"

void FRougeAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
}

void FRougeAbilitySet_GrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
}

void FRougeAbilitySet_GrantedHandles::AddAttributeSet(UAttributeSet* Set)
{
}

void FRougeAbilitySet_GrantedHandles::TakeFromAbilitySystem(URougeAbilitySystemComponent* RougeASC)
{
}

void URougeAbilitySet::GiveToAbilitySystem(URougeAbilitySystemComponent* RougeASC,
                                           FRougeAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
	check(RougeASC);

	//如果此组件的参与者具有权限，则返回true
	if (!RougeASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}

	//给予AbilitySystem
	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const FLyraAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];

		if (!IsValid(AbilityToGrant.Ability))
		{
			UE_LOG(LogLyraAbilitySystem, Error, TEXT("GrantedGameplayAbilities[%d] on ability set [%s] is not valid."), AbilityIndex, *GetNameSafe(this));
			continue;
		}

		ULyraGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<ULyraGameplayAbility>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = LyraASC->GiveAbility(AbilitySpec);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}

}
