// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RougeHeroComponent.h"

#include "EnhancedInputSubsystems.h"
#include "Character/RougePawnData.h"
#include "Character/RougePawnExtensionComponent.h"
#include "Core/RougeLocalPlayer.h"
#include "Core/RougePlayerState.h"
#include "RougeDemo/RougeGameplayTags.h"
#include "Core/RougePlayerController.h"

const FName URougeHeroComponent::NAME_BindInputsNow("BindInputsNow");
const FName URougeHeroComponent::NAME_ActorFeatureName("Hero");

URougeHeroComponent::URougeHeroComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

void URougeHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState)
{
	const FRougeGameplayTags& RougeGameplayTags = FRougeGameplayTags::Get();
	if (CurrentState == RougeGameplayTags.InitState_DataAvailable && DesiredState == RougeGameplayTags.InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		ARougePlayerState* RougePS = GetPlayerState<ARougePlayerState>();
		if (!ensure(Pawn && RougePS))
		{
			return;
		}

		const URougePawnData* PawnData = nullptr;
		if (URougePawnExtensionComponent* PawnExtComp = URougePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			//PawnData = PawnExtComp->GetPawnData<URougePawnData>();

			// PlayerState保存着该玩家的持久数据
			// AbilitySystemComponent和属性设置取决于玩家的状态。
			//PawnExtComp->InitializeAbilitySystem(RougePS->GetRougeAbilitySystemComponent(), RougePS);
		}

		if (ARougePlayerController* RougePC = GetController<ARougePlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}
	}
	
}

void URougeHeroComponent::CheckDefaultInitialization()
{
	const FRougeGameplayTags& InitTags = FRougeGameplayTags::Get();
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };
	
	ContinueInitStateChain(StateChain);
}

// Called when the game starts
void URougeHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	BindOnActorInitStateChanged(URougePawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	CheckDefaultInitialization();
}

void URougeHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void URougeHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}
	
	//获取Pawn的PlayerController，这在客户端通常为空
	const APlayerController* PC = GetController<APlayerController>();
	check(PC);
	
	const URougeLocalPlayer* LP = Cast<URougeLocalPlayer>(PC->GetLocalPlayer());
	check(LP);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();

	if (const URougePawnExtensionComponent* PawnExtComp = URougePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const URougePawnData* PawnData = PawnExtComp->GetPawnData<URougePawnData>())
		{
			if (const URougeInputConfig* InputConfig = PawnData->InputConfig)
			{
				const FRougeGameplayTags& GameplayTags = FRougeGameplayTags::Get();
				for (const FMappableConfigPair& Pair : DefaultInputConfigs)
				{
					if (Pair.bShouldActivateAutomatically && Pair.CanBeActivated())
					{
						FModifyContextOptions Options = {};
						Options.bIgnoreAllPressedKeysUntilRelease = false;
						// Actually add the config to the local player			
						Subsystem->AddPlayerMappableConfig(Pair.Config.LoadSynchronous(), Options);	
					}
				}

				// To do Input Component
			}
		}
	}
}

void URougeHeroComponent::OnRegister()
{
	Super::OnRegister();
}
