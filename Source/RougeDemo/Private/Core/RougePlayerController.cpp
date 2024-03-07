// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\Core\RougePlayerController.h"

#include "CommonInputSubsystem.h"
#include "CommonPlayerInputKey.h"
#include "GenericTeamAgentInterface.h"
#include "AbilitySystem/RougeAbilitySystemComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Character/RougeCharacter.h"
#include "Components/MagicComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Core/GameManager.h"
#include "Core/RougeLocalPlayer.h"
#include "HUD/PlayerOverlayWidget.h"
#include "HUD/RougeHUD.h"
#include "HUD/Game/LevelMasterWidget.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "Lib/RougeDemoFunctionLibary.h"
#include "RougeDemo/RougeDemo.h"
#include "Struct/AbilityLevelUp.h"
#include "Core/RougePlayerState.h"
#include "Kismet/KismetArrayLibrary.h"
#include "RougeDemo/RougeGameplayTags.h"
#include "SaveGame/RougeSettingsShared.h"
#include "Teams/RougeTeamAgentInterface.h"

DEFINE_LOG_CATEGORY(LogRougeController);

ARougePlayerController::ARougePlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ARougePlayerController::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ARougePlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetActorHiddenInGame(false);
	
	RougeDemoHUD = Cast<ARougeHUD>(GetHUD());

	FRougeGameplayTags GameplayTags = FRougeGameplayTags::Get();

	
}

void ARougePlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ARougePlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ARougePlayerController::InitPlayerState()
{
	Super::InitPlayerState();
	//PlayerState Init 完成
	BroadcastOnPlayerStateChanged();
}

void ARougePlayerController::CleanupPlayerState()
{
	Super::CleanupPlayerState();
}

void ARougePlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}

void ARougePlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
}

void ARougePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void ARougePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	SetIsAutoRunning(false);
}


void ARougePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

}

void ARougePlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	RougeDemoHUD = RougeDemoHUD == nullptr ? Cast<ARougeHUD>(GetHUD()) : RougeDemoHUD;
}

void ARougePlayerController::SetHUDScore(float Score)
{
	RougeDemoHUD = RougeDemoHUD == nullptr ? Cast<ARougeHUD>(GetHUD()) : RougeDemoHUD;

	bool bHUDValid = RougeDemoHUD &&
		RougeDemoHUD->PlayerOverlayWidget &&
		RougeDemoHUD->PlayerOverlayWidget->ScoreAmount;
	if(bHUDValid)
	{
		const FString ScoreAmountText = FString::Printf(TEXT("%d"),FMath::CeilToInt(Score));
		RougeDemoHUD->PlayerOverlayWidget->ScoreAmount->SetText(FText::FromString(ScoreAmountText));
	}
}

void ARougePlayerController::OnLevelUp()
{
	CreateLevelUpUI();
}

void ARougePlayerController::UpdateLevelBar(float Percent, int32 Level)
{
	UpdateCharacterUI(Percent, Level);
}

void ARougePlayerController::LevelUpMaxHealth(bool PowerUp)
{
	
}

void ARougePlayerController::LevelUpMaxSpeed(bool PowerUp)
{
}

void ARougePlayerController::LevelUpTimerReduction(bool PowerUp)
{
}

void ARougePlayerController::LevelUpAbilityDamage(bool PowerUp)
{
}

void ARougePlayerController::OnStartAutoRun()
{
	if (URougeAbilitySystemComponent* RougeASC = GetRougeAbilitySystemComponent())
	{
		RougeASC->SetLooseGameplayTagCount(FRougeGameplayTags::Get().Status_AutoRunning, 1);
		K2_OnStartAutoRun();
	}
}

void ARougePlayerController::OnEndAutoRun()
{
	if (URougeAbilitySystemComponent* RougeASC = GetRougeAbilitySystemComponent())
	{
		RougeASC->SetLooseGameplayTagCount(FRougeGameplayTags::Get().Status_AutoRunning, 0);
		K2_OnEndAutoRun();
	}
}

void ARougePlayerController::SetPlayer(UPlayer* InPlayer)
{
	Super::SetPlayer(InPlayer);
	
	if (const URougeLocalPlayer* RougeLocalPlayer = Cast<URougeLocalPlayer>(InPlayer))
	{
		URougeSettingsShared* UserSettings = RougeLocalPlayer->GetSharedSettings();
		UserSettings->OnSettingChanged.AddUObject(this, &ThisClass::OnSettingsChanged);

		OnSettingsChanged(UserSettings);
	}

	
}

void ARougePlayerController::PreProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PreProcessInput(DeltaTime, bGamePaused);
}

void ARougePlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (URougeAbilitySystemComponent* RougeASC = GetRougeAbilitySystemComponent())
	{
		RougeASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

void ARougePlayerController::UpdateForceFeedback(IInputInterface* InputInterface, const int32 ControllerId)
{
	if (bForceFeedbackEnabled)
	{
		if (const UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(GetLocalPlayer()))
		{
			const ECommonInputType CurrentInputType = CommonInputSubsystem->GetCurrentInputType();
			if (Rouge::Input::ShouldAlwaysPlayForceFeedback || CurrentInputType == ECommonInputType::Gamepad || CurrentInputType == ECommonInputType::Touch)
			{
				InputInterface->SetForceFeedbackChannelValues(ControllerId, ForceFeedbackValues);
				return;
			}
		}
	}
	
	InputInterface->SetForceFeedbackChannelValues(ControllerId, FForceFeedbackValues());
}

void ARougePlayerController::UpdateHiddenComponents(const FVector& ViewLocation,
	TSet<FPrimitiveComponentId>& OutHiddenComponents)
{
	if (bHideViewTargetPawnNextFrame)
	{
		AActor* const ViewTargetPawn = PlayerCameraManager ? Cast<AActor>(PlayerCameraManager->GetViewTarget()) : nullptr;
		if (ViewTargetPawn)
		{
			// internal helper func to hide all the components
			auto AddToHiddenComponents = [&OutHiddenComponents](const TInlineComponentArray<UPrimitiveComponent*>& InComponents)
			{
				// add every component and all attached children
				for (UPrimitiveComponent* Comp : InComponents)
				{
					if (Comp->IsRegistered())
					{
						OutHiddenComponents.Add(Comp->ComponentId);

						for (USceneComponent* AttachedChild : Comp->GetAttachChildren())
						{
							static FName NAME_NoParentAutoHide(TEXT("NoParentAutoHide"));
							UPrimitiveComponent* AttachChildPC = Cast<UPrimitiveComponent>(AttachedChild);
							if (AttachChildPC && AttachChildPC->IsRegistered() && !AttachChildPC->ComponentTags.Contains(NAME_NoParentAutoHide))
							{
								OutHiddenComponents.Add(AttachChildPC->ComponentId);
							}
						}
					}
				}
			};

			//TODO Solve with an interface.  Gather hidden components or something.
			//TODO Hiding isn't awesome, sometimes you want the effect of a fade out over a proximity, needs to bubble up to designers.

			// hide pawn's components
			TInlineComponentArray<UPrimitiveComponent*> PawnComponents;
			ViewTargetPawn->GetComponents(PawnComponents);
			AddToHiddenComponents(PawnComponents);

			//// hide weapon too
			//if (ViewTargetPawn->CurrentWeapon)
			//{
			//	TInlineComponentArray<UPrimitiveComponent*> WeaponComponents;
			//	ViewTargetPawn->CurrentWeapon->GetComponents(WeaponComponents);
			//	AddToHiddenComponents(WeaponComponents);
			//}
		}

		// we consumed it, reset for next frame
		bHideViewTargetPawnNextFrame = false;
	}
}

void ARougePlayerController::CreateLevelUpUI()
{
	if(HasAuthority())
	{
		bPreparingUI = true;
		bLevelUpHudUp = true;
		SetShowMouseCursor(true);
		PrepareLevelUp();
	}
}

void ARougePlayerController::PrepareLevelUp()
{
	if(LevelMasterWidgetClass == nullptr) { return; } 
	//创建LevelUpUI
	if(!LevelMasterWidget)
	{
		LevelMasterWidget = CreateWidget<ULevelMasterWidget>(this, LevelMasterWidgetClass);
		//暂停
		SetPause(true);
	}
	LevelMasterWidget->AddToViewport();

	//添加Ability卡片
	ExecuteLevelUp();
	
	//Bind Event to On Ready
	//LevelMasterWidget->OnReadyDelegate.BindUObject(this, &ARougePlayerController::ExecuteLevelUp);
	
	//ProcessLevelup Bind to OnClose 处理点击选择Card后
	LevelMasterWidget->OnCloseDelegate.BindUObject(this, &ARougePlayerController::ProcessLevelUp);
	//只能UI
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, LevelMasterWidget);
}

void ARougePlayerController::ExecuteLevelUp()
{
	TArray<FAbilityLevelUp> Local_ActiveAbilitiesArr;
	TArray<FAbilityLevelUp> Local_PassiveAbilitiesArr;

	//获取Rouge标签
	FRougeGameplayTags GameplayTags = FRougeGameplayTags::Get();

	//添加Active下的Local_ActiveAbilitiesArr
	for(FAbilityLevelUp AbilityLevelUp : ActiveAbilitiesArr)
	{
		//当前可升级的Ability是否为True状态，且不为最后一个Ability
		if(AbilityLevelUp.bActive)
		{
			Local_ActiveAbilitiesArr.Add(AbilityLevelUp);
		}
	}

	for(FAbilityLevelUp AbilityLevelUp : PassiveAbilitiesArr)
	{
		if(AbilityLevelUp.bActive)
		{
			Local_PassiveAbilitiesArr.Add(AbilityLevelUp);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Local_ActiveAbilitiesArr.Num[%d]"), Local_ActiveAbilitiesArr.Num());
	if(Local_ActiveAbilitiesArr.Num() > 0)
	{
		int RandomActiveIndex = UKismetMathLibrary::RandomInteger(Local_ActiveAbilitiesArr.Num());
		FAbilityLevelUp CurrentActiveAbility = Local_ActiveAbilitiesArr[RandomActiveIndex];

		FText ActiveAbilityType = UKismetTextLibrary::Conv_StringToText("ActiveAbility");
		CreateActiveCard(CurrentActiveAbility, CurrentActiveAbility.AbilityTag.GetByIndex(0), ActiveAbilityType);
	}

	if(Local_PassiveAbilitiesArr.Num() > 0)
	{
		int RandomPassiveIndex =  UKismetMathLibrary::RandomInteger(Local_PassiveAbilitiesArr.Num());
		FAbilityLevelUp CurrentPassiveAbility = Local_PassiveAbilitiesArr[RandomPassiveIndex];
	
		FText PassiveAbilityType = UKismetTextLibrary::Conv_StringToText("PassiveAbility");
		CreateActiveCard(CurrentPassiveAbility, CurrentPassiveAbility.AbilityTag.GetByIndex(0), PassiveAbilityType);
	}
}

bool ARougePlayerController::CheckIfEVOReady(EActiveAbilities& Ability)
{
	TArray<EPassiveAbilities> Local_PassiveArr = MagicComponent->EvolutionPassiveArr;
	TMap<EActiveAbilities, EPassiveAbilities> Local_EvoMap = MagicComponent->EvolutionMap;
	
	if(!Local_EvoMap.IsEmpty())
	{
		TArray<EActiveAbilities> OutKeys;
		Local_EvoMap.GetKeys(OutKeys);
		
		for(auto ActiveAbility : OutKeys)
		{
			if(Local_EvoMap.Contains(ActiveAbility))
			{
				EPassiveAbilities Passive = *Local_EvoMap.Find(ActiveAbility);
				if(Local_PassiveArr.Contains(Passive))
				{
					Ability = ActiveAbility;
					return true;
				}
			}
		}
	}

	Ability = EActiveAbilities::EAA_Hammer;
	return false;
}

TArray<EActiveAbilities> ARougePlayerController::CheckActiveAbilities(
	TMap<EActiveAbilities, int32> ActiveMap, int32 MaxLevel)
{
	int32 Local_MaxAbilityLevel = MaxLevel;
	TMap<EActiveAbilities, int32> Local_ActiveMap = ActiveMap; //<Hammer, 1>
	
	TArray<EActiveAbilities> LocalActiveAbilitiesArr;
	//已经激活的主动技能Map数量小于枚举中的技能数量
	//这里只有4个技能，数字要和枚举中的数量保持一致
	if(Local_ActiveMap.Num() < 4)
	{
		//用遍历枚举添加
		UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EActiveAbilities"), true);
		for(int i  = 0; i < EnumPtr->GetMaxEnumValue(); ++i)
		{
			int64 EnumMemberValue = EnumPtr->GetValueByIndex(i);
			LocalActiveAbilitiesArr.Add(static_cast<EActiveAbilities>(EnumMemberValue));
		}

		//删除已经满级的技能
		//Ex: 这里的满级是5，当某个主动技能5级时，则删除代表Card中不会再出现
		TArray<int32> OutValues;
		Local_ActiveMap.GenerateValueArray(OutValues);
		//1 
		for(auto Value : OutValues)
		{
			if(Value >= Local_MaxAbilityLevel)
			{
				TArray<EActiveAbilities> OutKeys;
				Local_ActiveMap.GetKeys(OutKeys);

				LocalActiveAbilitiesArr.Remove(OutKeys[Value]);
			}	
		}

		//返回任何尚未学习到满级主动技能
		return LocalActiveAbilitiesArr;
	}else
	{
		//已经激活的主动技能的Level
		TArray<int32> OutValues;
		Local_ActiveMap.GenerateValueArray(OutValues);
		for(auto Value : OutValues)
		{
			//当技能Level小于最大技能等级， 小于5 Ex：1 0 0 0
			if(Value < Local_MaxAbilityLevel)
			{
				TArray<EActiveAbilities> OutKeys;
				Local_ActiveMap.GetKeys(OutKeys);
				FString EnumString = UEnum::GetValueAsString(OutKeys[Value]);
				//CONTROLLER_LOG(Warning, TEXT("OutKeys[Value][%s]"), *EnumString)
				LocalActiveAbilitiesArr.Add(OutKeys[Value]);
			}	
		}
	}
	return LocalActiveAbilitiesArr;
}

TArray<EPassiveAbilities> ARougePlayerController::CheckPassiveAbilities(TMap<EPassiveAbilities, int32> PassiveMap,
	int32 MaxLevel)
{
	int32 Local_MaxAbilityLevel = MaxLevel;
	TMap<EPassiveAbilities, int32> Local_PassiveMap = PassiveMap;

	TArray<EPassiveAbilities> LocalPassiveAbilitiesArr;
	//已经激活的被动技能Map数量小于枚举中的技能数量
	if(Local_PassiveMap.Num() < 4)
	{
		//用遍历枚举添加
		UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EPassiveAbilities"), true);
		for(int i  = 0; i < EnumPtr->GetMaxEnumValue(); ++i)
		{
			int64 EnumMemberValue = EnumPtr->GetValueByIndex(i);
			LocalPassiveAbilitiesArr.Add(static_cast<EPassiveAbilities>(EnumMemberValue));
		}
		
		//删除已经满的技能
		TArray<int32> OutValues;
		Local_PassiveMap.GenerateValueArray(OutValues);
		for(auto Value : OutValues)
		{
			if(Value >= Local_MaxAbilityLevel)
			{
				TArray<EPassiveAbilities> OutKeys;
				Local_PassiveMap.GetKeys(OutKeys);

				LocalPassiveAbilitiesArr.Remove(OutKeys[Value]);
			}	
		}

		//返回任何尚未学习或已满的技能
		return LocalPassiveAbilitiesArr;
	}else
	{
		//删除已经满的技能
		TArray<int32> OutValues;
		Local_PassiveMap.GenerateValueArray(OutValues);
		for(auto Value : OutValues)
		{
			if(Value < Local_MaxAbilityLevel)
			{
				TArray<EPassiveAbilities> OutKeys;
				Local_PassiveMap.GetKeys(OutKeys);

				LocalPassiveAbilitiesArr.Add(OutKeys[Value]);
			}
		}
	}
	return LocalPassiveAbilitiesArr;
}

void ARougePlayerController::UpdateCharacterUI(float Percent, int32 Level)
{
	RougeDemoHUD = RougeDemoHUD == nullptr ? Cast<ARougeHUD>(GetHUD()) : RougeDemoHUD;

	if(RougeDemoHUD->PlayerOverlayWidget)
	{
		RougeDemoHUD->PlayerOverlayWidget->PB_XP->SetPercent(Percent);
		//const FString HealthText = FString::Printf(TEXT("%d/%d"),FMath::CeilToInt(Health),FMath::CeilToInt(MaxHealth));
		FText LevelText = FText::Format(FText::FromString(TEXT("Level: {0}")), Level);
		RougeDemoHUD->PlayerOverlayWidget->TB_Level->SetText(LevelText);
	}
}

void ARougePlayerController::TestAction()
{
	RougeDemoHUD = RougeDemoHUD == nullptr ? Cast<ARougeHUD>(GetHUD()) : RougeDemoHUD;

	if(RougeDemoHUD->PlayerOverlayWidget)
	{
		float Percent = RougeDemoHUD->PlayerOverlayWidget->PB_XP->GetPercent();
		
		TArray<AActor*> OutActorArr;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameManager::StaticClass(), OutActorArr);
		if(OutActorArr.Num() > 0)
		{
			AGameManager* GameManager = Cast<AGameManager>(OutActorArr[0]);
			Percent = Percent + 0.8f;
			Percent = UKismetMathLibrary::NormalizeToRange(Percent, 0.0f, 1.0f);
			GameManager->UpdateCharactersXP(Percent,GameManager->mLevel);
			if(Percent > 0.9)
			{
				Percent = 0.0f;
				GameManager->PrepareLevelUp();
			}
		}
	}
}

void ARougePlayerController::UpdateTime(FText Time)
{
	RougeDemoHUD = RougeDemoHUD == nullptr ? Cast<ARougeHUD>(GetHUD()) : RougeDemoHUD;
	if(RougeDemoHUD->PlayerOverlayWidget)
	{
		RougeDemoHUD->PlayerOverlayWidget->UpdateTime(Time);
	}
}

void ARougePlayerController::CreateActiveCard(FAbilityLevelUp AbilityLevelUp, FGameplayTag GameplayTag, FText AbilityType)
{
	FText AbilityName = AbilityLevelUp.AbilityName;
	FText AbilityDesc = AbilityLevelUp.AbilityDesc;
	UTexture2D* Icon = AbilityLevelUp.AbilityIcon;
	FString Level = GameplayTag.ToString();
	TArray<FString> LevelArr;
	Level.ParseIntoArray(LevelArr, TEXT("."), false);
	FText LevelText = UKismetTextLibrary::Conv_StringToText(LevelArr[LevelArr.Num()-1]);

	LevelMasterWidget->AddSelection(
		AbilityName,
		LevelText,
		AbilityDesc,
		Icon,
		AbilityType,
		GameplayTag
	);
}

void ARougePlayerController::ProcessLevelUp(FGameplayTag GameplayTag)
{
	bLevelUpHudUp = false;
	//是否触发了其他UI
	
	bPreparingUI = false;

	//继续游戏
	// ARougeDemoGameMode* GameMode = Cast<ARougeDemoGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	// if(GameMode)
	// {
	// 	if (IGameModeInterface* GameModeImpl = Cast<IGameModeInterface>(GameMode))
	// 	{
	// 		GameModeImpl->Pause(false,false);
	// 	}
	// }
	SetPause(false);
	
	SetShowMouseCursor(false);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
	
	//Level up ability
	AssignAbility(GameplayTag);
	//更新UI
	UpdateHotbar();
	
	//RefreshAbilities();
}

void ARougePlayerController::AssignAbility(FGameplayTag AbilityTag)
{
	UE_LOG(LogTemp, Warning, TEXT("AssignAbility().AbilityTag[%s]"), *AbilityTag.ToString());
	
	FRougeGameplayTags RougeGameplayTags = FRougeGameplayTags::Get();

	ARougePlayerState* PS = GetPlayerState<ARougePlayerState>();
	check(PS);

	URougeAbilitySystemComponent* RougeASC = PS->GetRougeAbilitySystemComponent();
	check(RougeASC);
	
	//UAbilitySystemComponent :: GiveAbility（...）方法将技能注册到AbilitySystemComponent
	FAbilityLevelUp Current_AbilityLevelUp;
	// 遍历整个主动Ability表
	for(FAbilityLevelUp& AbilityLevelUp : ActiveAbilitiesArr)
	{
		//检查ActiveAbility中是否等于当前所选择的Tag
		if(AbilityLevelUp.AbilityTag.HasTagExact(AbilityTag))
		{
			Current_AbilityLevelUp = AbilityLevelUp;
	
			URougeGameplayAbility* AbilityCDO = AbilityLevelUp.GameplayAbility.Ability->GetDefaultObject<URougeGameplayAbility>();
			FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityLevelUp.Level);

			//绑定按键
			FGameplayTag InputTag = AbilityLevelUp.InputTag.GetByIndex(0);
			//Log绑定按键
			//UE_LOG(LogTemp, Warning, TEXT("InputTag[%s]"), *InputTag.ToString());
			if(InputTag.IsValid())
			{
				AbilitySpec.DynamicAbilityTags.AddTag(AbilityLevelUp.InputTag.GetByIndex(0));
			}
			
			//赋予GameplayAbility
			RougeASC->GiveAbility(AbilitySpec);

			mAbilityMap.Add(AbilityTag, AbilityLevelUp);
			
			//设置激活为false，代表已激活过
			AbilityLevelUp.bActive = false;
		}
	}
	
	//下一个Level为true
	for(FAbilityLevelUp& NextLevelAbility : ActiveAbilitiesArr)
	{
		//Log
		//FString bActiveStr = LevelAbility.bActive ? "LevelAbility.true" : "LevelAbility.false";
		//UE_LOG(LogTemp, Warning, TEXT("LevelAbility[%s], LevelAbility.bActive[%s]"),*LevelAbility.AbilityTag.ToString() ,*bActiveStr);
		//更新下个Level的技能为true && !AbilityLevelUp.NextLevelAbilityTag.HasTagExact(GameplayTags.Ability_Type_Magic_Warrior_Max)
		if(NextLevelAbility.AbilityTag.HasTagExact(RougeGameplayTags.Ability_Type_Magic_Warrior_Max))
		{
			continue;
		}else if(NextLevelAbility.AbilityTag.HasTagExact(Current_AbilityLevelUp.NextLevelAbilityTag.GetByIndex(0)))
		{
			NextLevelAbility.bActive = true;
		}
	}
}

void ARougePlayerController::UpdateHudUI(TMap<FGameplayTag, FAbilityLevelUp>& AbilityMap)
{
	RougeDemoHUD = RougeDemoHUD == nullptr ? Cast<ARougeHUD>(GetHUD()) : RougeDemoHUD;
	if(RougeDemoHUD->PlayerOverlayWidget)
	{
		RougeDemoHUD->PlayerOverlayWidget->BuildHotbar(AbilityMap);
		//RougeDemoHUD->PlayerOverlayWidget->UpdateXP(0.f);

		TArray<AActor*> OutActorArr;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameManager::StaticClass(), OutActorArr);
		if(OutActorArr.Num() > 0)
		{
			AGameManager* GameManager = Cast<AGameManager>(OutActorArr[0]);
			GameManager->UpdateLevel(0.f);
		}
		
	}
}

void ARougePlayerController::BroadcastOnPlayerStateChanged()
{
	//OnPlayerStateChanged();

	//从旧的Player State中解除绑定(如果有的话)
	FGenericTeamId OldTeamID = FGenericTeamId::NoTeam;

	if (LastSeenPlayerState != nullptr)
	{
		if (IRougeTeamAgentInterface* PlayerStateTeamInterface = Cast<IRougeTeamAgentInterface>(LastSeenPlayerState))
		{
			OldTeamID = PlayerStateTeamInterface->GetGenericTeamId();
			PlayerStateTeamInterface->GetTeamChangedDelegateChecked().RemoveAll(this);
		}
	}
	
	// 绑定新的Player State(如果有的话)
	FGenericTeamId NewTeamID = FGenericTeamId::NoTeam;
	if (PlayerState != nullptr)
	{
		if (IRougeTeamAgentInterface* PlayerStateTeamInterface = Cast<IRougeTeamAgentInterface>(PlayerState))
		{
			NewTeamID = PlayerStateTeamInterface->GetGenericTeamId();
			PlayerStateTeamInterface->GetTeamChangedDelegateChecked().AddDynamic(this, &ThisClass::OnPlayerStateChangedTeam);
		}
	}
	
	ConditionalBroadcastTeamChanged(this, OldTeamID, NewTeamID);
	
	LastSeenPlayerState = PlayerState;
}

void ARougePlayerController::OnPlayerStateChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam)
{
	ConditionalBroadcastTeamChanged(this, IntegerToGenericTeamId(OldTeam), IntegerToGenericTeamId(NewTeam));
}

void ARougePlayerController::UpdateHotbar()
{
	if(mAbilityMap.Num() > 0)
	{
		UpdateHudUI(
			mAbilityMap
		);
	}
}

URougeAbilitySystemComponent* ARougePlayerController::GetRougeAbilitySystemComponent() const
{
	//从PlayerState中获取
	const ARougePlayerState* RougePS = GetRougePlayerState();
	return (RougePS ? RougePS->GetRougeAbilitySystemComponent() : nullptr);
}

ARougePlayerState* ARougePlayerController::GetRougePlayerState() const
{
	return CastChecked<ARougePlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

void ARougePlayerController::SetIsAutoRunning(const bool bEnabled)
{
	const bool bIsAutoRunning = GetIsAutoRunning();
	if (bEnabled != bIsAutoRunning)
	{
		if (!bEnabled)
		{
			OnEndAutoRun();
		}
		else
		{
			OnStartAutoRun();
		}
	}
}

bool ARougePlayerController::GetIsAutoRunning() const
{
	bool bIsAutoRunning = false;
	if (const URougeAbilitySystemComponent* RougeASC = GetRougeAbilitySystemComponent())
	{
		bIsAutoRunning = RougeASC->GetTagCount(FRougeGameplayTags::Get().Status_AutoRunning) > 0;
	}
	return bIsAutoRunning;
}

void ARougePlayerController::OnSettingsChanged(URougeSettingsShared* Settings)
{
	bForceFeedbackEnabled = Settings->GetForceFeedbackEnabled();
}

FOnRougeTeamIndexChangedDelegate* ARougePlayerController::GetOnTeamIndexChangedDelegate()
{
	return &OnTeamChangedDelegate;
}






