// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\Character\RougeManaComponent.h"

#include "AbilitySystem/RougeAbilitySystemComponent.h"
#include "AbilitySystem/Attribute/RougeHealthSet.h"
#include "RougeDemo/RougeGameplayTags.h"



URougeManaComponent::URougeManaComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


// Called when the game starts
void URougeManaComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void URougeManaComponent::InitializeWithAbilitySystem(URougeAbilitySystemComponent* InASC)
{
	AActor* Owner = GetOwner();
	check(Owner);

	if (AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("RougeHealthComponent: Health component for owner [%s] has already been initialized with an ability system."), *GetNameSafe(Owner));
		return;
	}

	AbilitySystemComponent = InASC;
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("RougeHealthComponent: Cannot initialize health component for owner [%s] with NULL ability system."), *GetNameSafe(Owner));
		return;
	}

	RougeSet = AbilitySystemComponent->GetSet<URougeHealthSet>();
	if (!RougeSet)
	{
		UE_LOG(LogTemp, Error, TEXT("RougeHealthComponent: Cannot initialize health component for owner [%s] with NULL health set on the ability system."), *GetNameSafe(Owner));
		return;
	}

	//当AttributeSet中的Value改变时，注册绑定事件
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URougeHealthSet::GetManaAttribute()).AddUObject(this, &ThisClass::HandleManaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URougeHealthSet::GetMaxManaAttribute()).AddUObject(this, &ThisClass::HandleMaxManaChanged);

	ClearGameplayTags();

	OnManaChanged.Broadcast(this, RougeSet->GetHealth(), RougeSet->GetHealth(), nullptr);
	OnMaxManaChanged.Broadcast(this, RougeSet->GetHealth(), RougeSet->GetHealth(), nullptr);
}


void URougeManaComponent::HandleManaChanged(const FOnAttributeChangeData& ChangeData)
{
	OnManaChanged.Broadcast(this, ChangeData.OldValue, ChangeData.NewValue, nullptr);
}

void URougeManaComponent::HandleMaxManaChanged(const FOnAttributeChangeData& ChangeData)
{
	OnMaxManaChanged.Broadcast(this, ChangeData.OldValue, ChangeData.NewValue, nullptr);
}

void URougeManaComponent::ClearGameplayTags()
{
	if (AbilitySystemComponent)
	{
		const FRougeGameplayTags& GameplayTags = FRougeGameplayTags::Get();

		AbilitySystemComponent->SetLooseGameplayTagCount(GameplayTags.Status_Death_Dying, 0);
		AbilitySystemComponent->SetLooseGameplayTagCount(GameplayTags.Status_Death_Dead, 0);
	}
}

float URougeManaComponent::GetMana() const
{
	return (RougeSet ? RougeSet->GetMana() : 0.0f);
}

float URougeManaComponent::GetMaxMana() const
{
	return (RougeSet ? RougeSet->GetMaxMana() : 0.0f);
}

float URougeManaComponent::GetManaNormalized() const
{
	if (RougeSet)
	{
		const float Mana = RougeSet->GetMana();
		const float MaxMana = RougeSet->GetMaxMana();

		return ((MaxMana > 0.0f) ? (Mana / MaxMana) : 0.0f);
	}

	return 0.0f;
}




