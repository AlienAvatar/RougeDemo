// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RougePawnExtensionComponent.h"
#include "../RougeGameplayTags.h"

URougePawnExtensionComponent::URougePawnExtensionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = nullptr;
}

// Called when the game starts
void URougePawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void URougePawnExtensionComponent::OnAbilitySystemInitialized_RegisterAndCall(
	FSimpleMulticastDelegate::FDelegate Delegate)
{
	//检查是否有任何函数绑定到给定的用户对象
	if(!OnAbilitySystemInitialized.IsBoundToObject(Delegate.GetUObject()))
	{
		// 将回调函数添加到多播委托中
		// 多播委托可以添加多个回调函数，当委托被触发时，所有注册的回调函数都会被调用
		OnAbilitySystemInitialized.Add(Delegate);
	}

	if(AbilitySystemComponent)
	{
		Delegate.Execute();
	}
}

void URougePawnExtensionComponent::OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate Delegate)
{
	if (!OnAbilitySystemUninitialized.IsBoundToObject(Delegate.GetUObject()))
	{
		OnAbilitySystemUninitialized.Add(Delegate);
	}
}


// Called every frame
void URougePawnExtensionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URougePawnExtensionComponent::SetupPlayerInputComponent()
{
	CheckDefaultInitialization();
}

void URougePawnExtensionComponent::CheckDefaultInitialization()
{
	// 检查其它进度是否初始化完成，因为在检查我们的进度之前，我们可能依赖的任何其他功能的进度
	CheckDefaultInitializationForImplementers();

	static const TArray<FGameplayTag> StateChain = {
		RougeGameplayTags::InitState_Spawned,
		// RougeGameplayTags::InitState_DataAvailable,
		// RougeGameplayTags::InitState_DataInitialized,
		// RougeGameplayTags::InitState_GameplayReady
	};

	//这将尝试从生成(只在BeginPlay中设置)到数据初始化阶段
	ContinueInitStateChain(StateChain);
}

