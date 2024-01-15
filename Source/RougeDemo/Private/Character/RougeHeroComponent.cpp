// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RougeHeroComponent.h"

#include "EnhancedInputSubsystems.h"
#include "Character/RougePawnData.h"
#include "Character/RougePawnExtensionComponent.h"
#include "Core/RougeLocalPlayer.h"
#include "Core/RougePlayerState.h"
#include "RougeDemo/RougeGameplayTags.h"
#include "Core/RougePlayerController.h"



URougeHeroComponent::URougeHeroComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

void URougeHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState)
{
	FRougeGameplayTags RougeGameplayTags = FRougeGameplayTags::Get();
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

// Called when the game starts
void URougeHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
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
			}
		}
	}
}

// Called every frame
void URougeHeroComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

