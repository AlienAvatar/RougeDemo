// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RougeHeroComponent.h"

#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/RougeAbilitySystemComponent.h"
#include "Character/RougePawnData.h"
#include "Character/RougePawnExtensionComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Core/RougeLocalPlayer.h"
#include "Core/RougePlayerState.h"
#include "RougeDemo/RougeGameplayTags.h"
#include "Core/RougePlayerController.h"
#include "Input/RougeInputComponent.h"
#include "EnhancedInputSubsystems.h"

const FName URougeHeroComponent::NAME_BindInputsNow("BindInputsNow");
const FName URougeHeroComponent::NAME_ActorFeatureName("Hero");

URougeHeroComponent::URougeHeroComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	bReadyToBindInputs = false;
}

// Called when the game starts
void URougeHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//以侦听初始状态更改
	BindOnActorInitStateChanged(URougePawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);
	ensure(TryToChangeInitState(FRougeGameplayTags::Get().InitState_Spawned));

	//以尝试遵循4状态初始化链
	CheckDefaultInitialization();
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
	const FRougeGameplayTags& InitTags = FRougeGameplayTags::Get();
	if (CurrentState == FRougeGameplayTags::Get().InitState_DataAvailable && DesiredState == FRougeGameplayTags::Get().InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		ARougePlayerState* RougePS = GetPlayerState<ARougePlayerState>();
		if (!ensure(Pawn && RougePS))
		{
			return;
		}

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const URougePawnData* PawnData = nullptr;
		if (URougePawnExtensionComponent* PawnExtComp = URougePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<URougePawnData>();

			// PlayerState保存着该玩家的持久数据
			// AbilitySystemComponent和属性设置取决于玩家的状态。
			PawnExtComp->InitializeAbilitySystem(RougePS->GetRougeAbilitySystemComponent(), RougePS);
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
	//状态链
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };

	//尝试遵循连接的init状态链，按顺序执行状态并返回到达的最终状态
	ContinueInitStateChain(StateChain);
}

void URougeHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//以从系统注销状态并取消绑定委托
	UnregisterInitStateFeature();
	
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
			//读取PawnData中的数据（默认为InputData_SimplePawn）
			if (const URougeInputConfig* InputConfig = PawnData->InputConfig)
			{
				const FRougeGameplayTags& GameplayTags = FRougeGameplayTags::Get();
				//需要在Pawn中的HeroComponent中配置
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

				//绑定Ability Action的Pressed和Released
				TArray<uint32> BindHandles;
				RougeIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);

				//绑定Native Action
				RougeIC->AddInputMappings(InputConfig, Subsystem);
 				RougeIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, /*bLogIfNotFound=*/ false);
				RougeIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, /*bLogIfNotFound=*/ false);
				//RougeIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Test, ETriggerEvent::Triggered, this, &ThisClass::Input_Test, /*bLogIfNotFound=*/ false);
				//RougeIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Look_Stick, ETriggerEvent::Triggered, this, &ThisClass::Input_LookStick, /*bLogIfNotFound=*/ false);
			}
		}
	}
}

void URougeHeroComponent::RemoveAdditionalInputConfig(const URougeInputConfig* InputConfig)
{
	//To do something
}

bool URougeHeroComponent::IsReadyToBindInputs() const
{
	return bReadyToBindInputs;
}

void URougeHeroComponent::AddAdditionalInputConfig(const URougeInputConfig* InputConfig)
{
	TArray<uint32> BindHandles;

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	URougeInputComponent* RougeIC = Pawn->FindComponentByClass<URougeInputComponent>();
	check(RougeIC);

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	if (const URougePawnExtensionComponent* PawnExtComp = URougePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		//绑定Ability输入设置
		RougeIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);
	}
}

void URougeHeroComponent::OnRegister()
{
	Super::OnRegister();

	//如果没有挂载到Pawn上，则无法注册, HeroComponent只能在Pawn上使用
	if (!GetPawn<APawn>())
	{
		UE_LOG(LogTemp, Error, TEXT("[URougeHeroComponent::OnRegister] This component has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint."));
	}else
	{
		//尽早注册init State，这只会在GameWorld中起作用
		//向系统注册，但不设置状态
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
		Pawn->AddControllerPitchInput(Value.Y * (-1));
	}
}

void URougeHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	// If the player has attempted to move again then cancel auto running
	if (ARougePlayerController* RougeController = Cast<ARougePlayerController>(Controller))
	{
		RougeController->SetIsAutoRunning(false);
	}
	
	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void URougeHeroComponent::Input_Test()
{
	UE_LOG(LogTemp, Warning, TEXT("Input_Test"));
}

void URougeHeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const URougePawnExtensionComponent* PawnExtComp = URougePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (URougeAbilitySystemComponent* RougeASC = PawnExtComp->GetRougeAbilitySystemComponent())
			{
				RougeASC->AbilityInputTagPressed(InputTag);
			}
		}	
	}
}

void URougeHeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	if (const URougePawnExtensionComponent* PawnExtComp = URougePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (URougeAbilitySystemComponent* RougeASC = PawnExtComp->GetRougeAbilitySystemComponent())
		{
			RougeASC->AbilityInputTagReleased(InputTag);
		}
	}
}


