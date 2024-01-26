// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RougePawnExtensionComponent.h"
#include "../RougeGameplayTags.h"
#include "UObject/UObjectBaseUtility.h"

const FName URougePawnExtensionComponent::NAME_ActorFeatureName("PawnExtension");

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
	// 绑定监听所有Feature，是否有改变
	//调用将OnActorInitStateChanged函数绑定到组件管理器上的适当委托
	BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);

	//如果初始状态可能改变为InitState_Spawned状态的话，通知状态Manager，默认初始化其余部分
	ensure(TryToChangeInitState(FRougeGameplayTags::Get().InitState_Spawned));
	CheckDefaultInitialization();
}

void URougePawnExtensionComponent::SetPawnData(const URougePawnData* InPawnData)
{
	check(InPawnData);

	APawn* Pawn = GetPawnChecked<APawn>();

	if (Pawn->GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		//UE_LOG(LogTemp, Error, TEXT("Trying to set PawnData [%s] on pawn [%s] that already has valid PawnData [%s]."), *GetNameSafe(InPawnData), *GetNameSafe(Pawn), *GetNameSafe(PawnData));
		return;
	}
	
	PawnData = InPawnData;

	Pawn->ForceNetUpdate();

	CheckDefaultInitialization();
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


// void URougePawnExtensionComponent::OnRep_PawnData()
// {
// }

void URougePawnExtensionComponent::HandlePlayerStateReplicated()
{
	CheckDefaultInitialization();
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
	// UGameFrameworkInitStateInterface的方法
	CheckDefaultInitializationForImplementers();

	const FRougeGameplayTags& InitTags = FRougeGameplayTags::Get();
 	static const TArray<FGameplayTag> StateChain = {
		InitTags.InitState_Spawned,
		InitTags.InitState_DataAvailable,
		InitTags.InitState_DataInitialized,
		InitTags.InitState_GameplayReady
	};
	
	//这将尝试从生成(只在BeginPlay中设置)到数据初始化阶段
	ContinueInitStateChain(StateChain);
}

void URougePawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	// If another feature is now in DataAvailable, see if we should transition to DataInitialized
	//NAME_ActorFeatureName为PawnExtension
	if (Params.FeatureName != NAME_ActorFeatureName)
	{
		const FRougeGameplayTags& InitTags = FRougeGameplayTags::Get();
		if (Params.FeatureState == InitTags.InitState_DataAvailable)
		{
			CheckDefaultInitialization();
		}
	}
}

