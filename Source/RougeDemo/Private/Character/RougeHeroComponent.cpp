// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RougeHeroComponent.h"

#include "EnhancedInputSubsystems.h"
#include "Character/RougePawnData.h"
#include "Character/RougePawnExtensionComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Core/RougeLocalPlayer.h"
#include "Core/RougePlayerState.h"
#include "RougeDemo/RougeGameplayTags.h"
#include "Core/RougePlayerController.h"
#include "Input/RougeInputComponent.h"

const FName URougeHeroComponent::NAME_BindInputsNow("BindInputsNow");
const FName URougeHeroComponent::NAME_ActorFeatureName("Hero");

URougeHeroComponent::URougeHeroComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

bool URougeHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState) const
{
	check(Manager);

	const FRougeGameplayTags& InitTags = FRougeGameplayTags::Get();
	APawn* Pawn = GetPawn<APawn>();

	//如果状态为InitState_Spawned(已经生成过了)，则返回true
	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		// As long as we have a real pawn, let us transition
		if (Pawn)
		{
			return true;
		}
	}
	//当前状态为InitState_Spawned, 期望状态InitState_DataAvailable
	else if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		// The player state is required.
		//PlayerState是必须的
		if (!GetPlayerState<ARougePlayerState>())
		{
			return false;
		}

		// If we're authority or autonomous, we need to wait for a controller with registered ownership of the player state.
		if (Pawn->GetLocalRole() != ROLE_SimulatedProxy)
		{
			AController* Controller = GetController<AController>();

			//当前PlayerState不能为空，而且Controller必须相等
			const bool bHasControllerPairedWithPS = (Controller != nullptr) && \
				(Controller->PlayerState != nullptr) && \
				(Controller->PlayerState->GetOwner() == Controller);

			if (!bHasControllerPairedWithPS)
			{
				return false;
			}
		}

		//必须是客户端控制且不为AI
		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const bool bIsBot = Pawn->IsBotControlled();

		if (bIsLocallyControlled && !bIsBot)
		{
			ARougePlayerController* RougePC = GetController<ARougePlayerController>();

			// The input component and local player is required when locally controlled.
			if (!Pawn->InputComponent || !RougePC || !RougePC->GetLocalPlayer())
			{
				return false;
			}
		}

		return true;
	}
	else if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		// Wait for player state and extension component
		ARougePlayerState* RougePS = GetPlayerState<ARougePlayerState>();

		return RougePS && Manager->HasFeatureReachedInitState(Pawn, URougePawnExtensionComponent::NAME_ActorFeatureName, InitTags.InitState_DataInitialized);
	}
	else if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		// TODO add ability initialization checks?
		//添加技能
		return true;
	}
	return false;
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

void URougeHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName == URougePawnExtensionComponent::NAME_ActorFeatureName)
	{
		if (Params.FeatureState == FRougeGameplayTags::Get().InitState_DataInitialized)
		{
			// If the extension component says all all other components are initialized, try to progress to next state
			CheckDefaultInitialization();
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
	ensure(TryToChangeInitState(FRougeGameplayTags::Get().InitState_Spawned));
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
				//这里做绑定
				URougeInputComponent* RougeIC = CastChecked<URougeInputComponent>(PlayerInputComponent);

				RougeIC->AddInputMappings(InputConfig, Subsystem);
				RougeIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, /*bLogIfNotFound=*/ false);
				//RougeIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Look_Stick, ETriggerEvent::Triggered, this, &ThisClass::Input_LookStick, /*bLogIfNotFound=*/ false);
			}
		}
	}
}

void URougeHeroComponent::RemoveAdditionalInputConfig(const URougeInputConfig* InputConfig)
{
	//To do something
}

void URougeHeroComponent::OnRegister()
{
	Super::OnRegister();

	//如果没有挂载到Pawn上，则无法注册
	if (!GetPawn<APawn>())
	{
		UE_LOG(LogTemp, Error, TEXT("[URougeHeroComponent::OnRegister] This component has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint."));
	}else
	{
		//尽早注册init State，这只会在游戏世界中起作用
		RegisterInitStateFeature();
	}
}

void URougeHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return;
	}
	
	const FVector2D Value = InputActionValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y);
	}
}


