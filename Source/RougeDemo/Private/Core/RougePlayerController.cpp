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
	SetupPlayer();
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

	//InputComponent->BindAction("Test", IE_Pressed, this, &ARougePlayerController::TestAction);
}

void ARougePlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	RougeDemoHUD = RougeDemoHUD == nullptr ? Cast<ARougeHUD>(GetHUD()) : RougeDemoHUD;

	bool bHUDValid = RougeDemoHUD &&
		RougeDemoHUD->PlayerOverlayWidget &&
		RougeDemoHUD->PlayerOverlayWidget->HealthBar &&
		RougeDemoHUD->PlayerOverlayWidget->HealthText;

	if(bHUDValid)
	{
		const float HealthPercent = Health / MaxHealth;
		RougeDemoHUD->PlayerOverlayWidget->HealthBar->SetPercent(HealthPercent);
		const FString HealthText = FString::Printf(TEXT("%d/%d"),FMath::CeilToInt(Health),FMath::CeilToInt(MaxHealth));
		RougeDemoHUD->PlayerOverlayWidget->HealthText->SetText(FText::FromString(HealthText));
	}
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

void ARougePlayerController::SetHUDSkillValue(float SkillValue,float MaxSkillValue)
{
	RougeDemoHUD = RougeDemoHUD == nullptr ? Cast<ARougeHUD>(GetHUD()) : RougeDemoHUD;

	bool bHUDValid = RougeDemoHUD &&
		RougeDemoHUD->PlayerOverlayWidget &&
		RougeDemoHUD->PlayerOverlayWidget->SkillValueBar;

	if(bHUDValid)
	{
		const float HealthPercent = SkillValue / MaxSkillValue;
		RougeDemoHUD->PlayerOverlayWidget->HealthBar->SetPercent(HealthPercent);
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

void ARougePlayerController::SetupPlayer()
{
	//检查是否是本地
	if(IsLocalPlayerController())
	{
		ARougeCharacter* RougeDemoCharacter = Cast<ARougeCharacter>(GetPawn());
		if(RougeDemoCharacter)
		{
			AbilityComponent = RougeDemoCharacter->GetAbilityComponent();
			if(AbilityComponent)
			{
				//初始化技能
				AbilityComponent->SetStartingAbility();
			}
		}
	}
}

void ARougePlayerController::SetReference()
{
	
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
		
		//添加卡片
		ExecuteLevelUp();
		//Bind Event to On Ready
		LevelMasterWidget->OnReadyDelegate.BindUObject(this, &ARougePlayerController::ExecuteLevelUp);
		//ProcessLevelup Bind to OnClose 处理点击选择后
		LevelMasterWidget->OnCloseDelegate.BindUObject(this, &ARougePlayerController::ProcessLevelUp);
	}else
	{
		LevelMasterWidget->ResetUI();
	}
	LevelMasterWidget->AddToViewport();
	//只能UI
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, LevelMasterWidget);
}

void ARougePlayerController::ExecuteLevelUp()
{
	if(AbilityComponent == nullptr) { return; }

	//保存为本地变量，记录角色每个技能的技能情况
	TMap<EActiveAbilities, int32> Local_ActiveAbilitiesMap = AbilityComponent->ActiveAbilitiesMap;
	TMap<EPassiveAbilities, int32> Local_PassiveAbilitiesMap = AbilityComponent->PassiveAbilitiesMap;
	int32 Local_MaxAbilityLevel = AbilityComponent->MaxAbilityLevel;

	//检查是否有可以升级的主动技能
	EActiveAbilities Local_EvoAbility;
	bool bLocal_EvoReady = CheckIfEVOReady(Local_EvoAbility);

	//设置最大的展示CardList
	int32 Local_MaxCount = 4;

	int Local_CardCount = 0;
	
	//如果Local_EvoReady = true， 升级技能
	if(bLocal_EvoReady)
	{
		++Local_CardCount;

		//检查升级的技能是否和表中的相等
		TArray<FName> OutRowNames;
		UDataTableFunctionLibrary::GetDataTableRowNames(DT_ActiveAbilities, OutRowNames);
		
		for(FName RowName : OutRowNames)
		{
			FText RowNameText = UKismetTextLibrary::Conv_NameToText(RowName);
			
			FString EvoAbilityStr = UEnum::GetValueAsString(Local_EvoAbility);
			const FText EvoAbilityText = UKismetTextLibrary::Conv_StringToText(EvoAbilityStr);
			int32 Num = UKismetSystemLibrary::MakeLiteralInt(-1);
			FText LevelNum = UKismetTextLibrary::Conv_IntToText(Num);
			FText SkillLevelText = FText::Format(FText::FromString(TEXT("{0}{1}")), EvoAbilityText, LevelNum);

			//相等的话，获取表中数据
			if(SkillLevelText.EqualTo(RowNameText))
			{
				FAbilityLevelUp* AbilityLevelUp = DT_ActiveAbilities->FindRow<FAbilityLevelUp>(RowName, "");
				if(AbilityLevelUp)
				{
					LevelMasterWidget->AddSelection(
						EvoAbilityText,
						-1,
						AbilityLevelUp->LevelUpText,
						URougeDemoFunctionLibary::FindActionIcon(Local_EvoAbility),
						Local_EvoAbility,
						EPassiveAbilities::EPA_AbilityDamage,
						EAbilityType::EAT_Evolution
					);
				}
			}
		}
	}

	//获得其他技能，防止满级技能出现
	CanAddActiveAbility = true;
	//传入Local_MaxAbilityLevel = 5
	TArray<EActiveAbilities> Local_AvailableActiveAbilities = CheckActiveAbilities(Local_ActiveAbilitiesMap, Local_MaxAbilityLevel);
	//如果Local_AvailableActiveAbilities为空，证明所有主动技能都学习到满级
	if(Local_AvailableActiveAbilities.IsEmpty())
	{
		CanAddActiveAbility = false;
	}
	
	//检查被动技能
	CanAddPassiveAbility = true;
	TArray<EPassiveAbilities> Local_AvailablePassiveAbilities = CheckPassiveAbilities(Local_PassiveAbilitiesMap, Local_MaxAbilityLevel);
	if(Local_AvailablePassiveAbilities.IsEmpty())
	{
		CanAddPassiveAbility = false;
	}
	
	//运行循环，直到我们打出我们想要的卡牌或不能添加被动/主动技能
	while((Local_CardCount < Local_MaxCount) && (CanAddActiveAbility || CanAddPassiveAbility))
	{
		++Local_CardCount;
		//CONTROLLER_LOG(Warning, TEXT("Local_CardCount[%d]"), Local_CardCount); 4
		//确定主动或被动的随机bool，如果不允许，则为failsafe, 50%概率添加4个主动技能或4个被动技能
		//当主动技能添加满时（所有主动技能都是满级），则添加被动技能
		//Test
		// if(CanAddPassiveAbility)
		// {
		// 	CreatePassiveCard(
		// 		Local_MaxCount,
		// 		Local_AvailablePassiveAbilities,
		// 		Local_PassiveAbilitiesMap
		// 	);
		// }
		
		if(UKismetMathLibrary::RandomBoolWithWeight(0.5))
		{
			if(CanAddActiveAbility)
			{
				//4, 
				CreateActiveCard(
					Local_MaxCount,
					Local_AvailableActiveAbilities,
					Local_ActiveAbilitiesMap
				);
			}else
			{
				CreatePassiveCard(
					Local_MaxCount,
					Local_AvailablePassiveAbilities,
					Local_PassiveAbilitiesMap
				);
			}
		}else
		{
			if(CanAddPassiveAbility)
			{
				CreatePassiveCard(
					Local_MaxCount,
					Local_AvailablePassiveAbilities,
					Local_PassiveAbilitiesMap
				);
			}else
			{
				CreateActiveCard(
					Local_MaxCount,
					Local_AvailableActiveAbilities,
					Local_ActiveAbilitiesMap
				);
			}
		}
	}
	//如果没有技能可以添加，就加血
	
}

bool ARougePlayerController::CheckIfEVOReady(EActiveAbilities& Ability)
{
	TArray<EPassiveAbilities> Local_PassiveArr = AbilityComponent->EvolutionPassiveArr;
	TMap<EActiveAbilities, EPassiveAbilities> Local_EvoMap = AbilityComponent->EvolutionMap;
	
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
			GameManager->UpdateCharactersXP(Percent, 2);
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

void ARougePlayerController::CreateActiveCard(int32 Local_MaxCount, TArray<EActiveAbilities>& Local_AvailableActiveAbilities,
	TMap<EActiveAbilities, int32>& Local_ActiveAbilitiesMap)
{
	EActiveAbilities Local_ActiveAbility;
	//随机选择一个随机技能
	int32 RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, Local_AvailableActiveAbilities.Num() - 1); //0 || 1 || 2 || 3
    Local_ActiveAbility = Local_AvailableActiveAbilities[RandomIndex];
	//从可升级技能数组中删除，防止产生重复技能的Card了
   	Local_AvailableActiveAbilities.Remove(Local_ActiveAbility);
 	CanAddActiveAbility = !Local_AvailableActiveAbilities.IsEmpty();
	
	//从DataTable中获取技能信息
	TArray<FName> OutRowNames;
	UDataTableFunctionLibrary::GetDataTableRowNames(DT_ActiveAbilities, OutRowNames);
	
	for(FName RowName : OutRowNames)
	{
		FText RowNameText = UKismetTextLibrary::Conv_NameToText(RowName);
		
		//设置Level
		int32 Level = 0;
		//当Map中包括Local_ActiveAbility代表该技能已经激活
		if(Local_ActiveAbilitiesMap.Contains(Local_ActiveAbility))
		{
			Level = Local_ActiveAbilitiesMap[Local_ActiveAbility];
			++Level;
		}

		//拆解字符串
		FString EvoAbilityStr = UEnum::GetValueAsString(Local_ActiveAbility);
		int32 Index_Local_ActiveAbility = EvoAbilityStr.Find("_");
		EvoAbilityStr = UKismetStringLibrary::GetSubstring(EvoAbilityStr, Index_Local_ActiveAbility + 1,EvoAbilityStr.Len());
		const FText EvoAbilityText = UKismetTextLibrary::Conv_StringToText(EvoAbilityStr);
		FText LevelText = UKismetTextLibrary::Conv_IntToText(Level);
		FText SkillLevelText = FText::Format(FText::FromString(TEXT("{0}{1}")), EvoAbilityText, LevelText);
		
		//检查升级的卡片是否和表中的相等
		if(SkillLevelText.EqualTo(RowNameText))
		{
			FAbilityLevelUp* AbilityLevelUp = DT_ActiveAbilities->FindRow<FAbilityLevelUp>(RowName, "");
			if(AbilityLevelUp)
			{
				//添加到UI
				LevelMasterWidget->AddSelection(
					EvoAbilityText,
					Level,
					AbilityLevelUp->LevelUpText,
					URougeDemoFunctionLibary::FindActionIcon(Local_ActiveAbility),
					Local_ActiveAbility,
					EPassiveAbilities::EPA_AbilityDamage,
					EAbilityType::EAT_Active
				);
				break;
			}
		}
	}
}

void ARougePlayerController::CreatePassiveCard(int32 Local_MaxCount,
	TArray<EPassiveAbilities>& Local_AvailablePassiveAbilities,TMap<EPassiveAbilities, int32>& Local_PassiveAbilitiesMap)
{
	//从本地数组中删除，这样我们就没有重复项了
	EPassiveAbilities Local_PassiveAbility;
	int32 RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, Local_AvailablePassiveAbilities.Num() - 1);
				
	Local_PassiveAbility = Local_AvailablePassiveAbilities[RandomIndex];
	
	Local_AvailablePassiveAbilities.Remove(Local_PassiveAbility);
	CanAddPassiveAbility = !Local_AvailablePassiveAbilities.IsEmpty();
	//检查升级的技能是否和表中的相等
	TArray<FName> OutRowNames;
	UDataTableFunctionLibrary::GetDataTableRowNames(DT_PassiveAbilities, OutRowNames);

	for(FName RowName : OutRowNames)
	{
		FText RowNameText = UKismetTextLibrary::Conv_NameToText(RowName);
		//CONTROLLER_LOG(Warning, TEXT("RowNameText[%s]"), *RowNameText.ToString());
		
		//设置Level
		int32 Level = 0;
		if(Local_PassiveAbilitiesMap.Contains(Local_PassiveAbility))
		{
			Level = Local_PassiveAbilitiesMap[Local_PassiveAbility];
			++Level;
		}
					
		FString EvoAbilityStr = UEnum::GetValueAsString(Local_PassiveAbility);
		int32 Index_Local_PassiveAbility = EvoAbilityStr.Find("_");
		EvoAbilityStr = UKismetStringLibrary::GetSubstring(EvoAbilityStr, Index_Local_PassiveAbility + 1,EvoAbilityStr.Len());
		const FText EvoAbilityText = UKismetTextLibrary::Conv_StringToText(EvoAbilityStr);
		// const FText LevelText = UKismetTextLibrary::Conv_IntToText(Level);
		// FText SkillLevelText = FText::Format(FText::FromString(TEXT("{0}{1}")), EvoAbilityText, Level);
		// CONTROLLER_LOG(Warning, TEXT("SkillLevelText[%s]"), *SkillLevelText.ToString());
		
		//相等的话，从DataTable中获取技能信息
		if(EvoAbilityText.EqualTo(RowNameText))
		{
			FAbilityLevelUp* AbilityLevelUp = DT_PassiveAbilities->FindRow<FAbilityLevelUp>(RowName, "");

			if(AbilityLevelUp)
			{
				//添加到UI
				LevelMasterWidget->AddSelection(
					EvoAbilityText,
					Level,
					AbilityLevelUp->LevelUpText,
					URougeDemoFunctionLibary::FindPassiveIcon(Local_PassiveAbility),
					EActiveAbilities::EAA_Hammer,
					Local_PassiveAbility,
					EAbilityType::EAT_Passive
				);
				break;
			}
		}
	}
}

void ARougePlayerController::ProcessLevelUp(EAbilityType Type, EActiveAbilities ActiveAbilities, EPassiveAbilities PassiveAbilities)
{
	FString EnumName = UEnum::GetValueAsString(Type);
	UE_LOG(LogTemp, Warning, TEXT("EAbilityType[%s]"), *EnumName);

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
	AssignAbility(Type, ActiveAbilities, PassiveAbilities);

	//更新UI
	UpdateHotbar();
	
	//RefreshAbilities();
	AbilityComponent->RefreshAbilities();
}

void ARougePlayerController::AssignAbility(EAbilityType Type, EActiveAbilities ActiveAbilities,
	EPassiveAbilities PassiveAbilities)
{
	switch (Type)
	{
		case EAbilityType::EAT_Active:
			switch (ActiveAbilities)
			{
				case EActiveAbilities::EAA_Hammer:
					AbilityComponent->LevelUpHammer();
					break;
				case EActiveAbilities::EAA_FireBall:
					
					break;
				case EActiveAbilities::EAA_Lightning:
					AbilityComponent->LevelUpLightning();
					break;
				case EActiveAbilities::EAA_FrostBolt:
					AbilityComponent->LevelUpFrostBolt();
					break;
			}
			break;
		case EAbilityType::EAT_Passive:
			switch (PassiveAbilities)
			{
				case EPassiveAbilities::EPA_Health:
					AbilityComponent->LevelUpMaxHealth(false);
					break;
				case EPassiveAbilities::EPA_Speed:
					AbilityComponent->LevelUpWalkSpeed(false);
					break;
				case EPassiveAbilities::EPA_AbilityDamage:
					AbilityComponent->LevelUpAbilityDamage(false);
					break;
				case EPassiveAbilities::EPA_CooldownReduction:
					AbilityComponent->LevelUpTimerReduction(false);
					break;
			}
			break;
		case EAbilityType::EAT_Evolution:
			break;
		case EAbilityType::EAT_Health:
			{
				ICharacterInterface* CharacterImpl = Cast<ICharacterInterface>(GetPawn());
				if(CharacterImpl)
				{
					CharacterImpl->RestoreHealth(10.f);
				}
				break;
			}
		case EAbilityType::EAT_Gold:
			break;
	}
}

void ARougePlayerController::UpdateHudHotbar(TMap<EActiveAbilities, int32> ActiveMap,
	TMap<EPassiveAbilities, int32> PassiveMap)
{
	RougeDemoHUD = RougeDemoHUD == nullptr ? Cast<ARougeHUD>(GetHUD()) : RougeDemoHUD;
	if(RougeDemoHUD->PlayerOverlayWidget)
	{
		RougeDemoHUD->PlayerOverlayWidget->BuildHotbar(ActiveMap, PassiveMap);
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
	if(AbilityComponent == nullptr) { return; }
	UpdateHudHotbar(
		AbilityComponent->ActiveAbilitiesMap,
		AbilityComponent->PassiveAbilitiesMap
	);
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






