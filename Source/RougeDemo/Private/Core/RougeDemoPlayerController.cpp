// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/RougeDemoPlayerController.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Character/RougeDemoCharacter.h"
#include "Components/AbilityComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Core/GameManager.h"
#include "Core/RougeDemoGameMode.h"
#include "HUD/PlayerOverlayWidget.h"
#include "HUD/RougeDemoHUD.h"
#include "HUD/Game/LevelMasterWidget.h"
#include "Interface/GameModeInterface.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "Lib/RougeDemoFunctionLibary.h"
#include "RougeDemo/RougeDemo.h"
#include "Struct/AbilityLevelUp.h"

DEFINE_LOG_CATEGORY(LogRougeController);

void ARougeDemoPlayerController::BeginPlay()
{
	Super::BeginPlay();

	RougeDemoHUD = Cast<ARougeDemoHUD>(GetHUD());
	SetupPlayer();
}



void ARougeDemoPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Test", IE_Pressed, this, &ARougeDemoPlayerController::TestAction);
}

void ARougeDemoPlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	RougeDemoHUD = RougeDemoHUD == nullptr ? Cast<ARougeDemoHUD>(GetHUD()) : RougeDemoHUD;

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

void ARougeDemoPlayerController::SetHUDScore(float Score)
{
	RougeDemoHUD = RougeDemoHUD == nullptr ? Cast<ARougeDemoHUD>(GetHUD()) : RougeDemoHUD;

	bool bHUDValid = RougeDemoHUD &&
		RougeDemoHUD->PlayerOverlayWidget &&
		RougeDemoHUD->PlayerOverlayWidget->ScoreAmount;
	if(bHUDValid)
	{
		const FString ScoreAmountText = FString::Printf(TEXT("%d"),FMath::CeilToInt(Score));
		RougeDemoHUD->PlayerOverlayWidget->ScoreAmount->SetText(FText::FromString(ScoreAmountText));
	}
}

void ARougeDemoPlayerController::SetHUDSkillValue(float SkillValue,float MaxSkillValue)
{
	RougeDemoHUD = RougeDemoHUD == nullptr ? Cast<ARougeDemoHUD>(GetHUD()) : RougeDemoHUD;

	bool bHUDValid = RougeDemoHUD &&
		RougeDemoHUD->PlayerOverlayWidget &&
		RougeDemoHUD->PlayerOverlayWidget->SkillValueBar;

	if(bHUDValid)
	{
		const float HealthPercent = SkillValue / MaxSkillValue;
		RougeDemoHUD->PlayerOverlayWidget->HealthBar->SetPercent(HealthPercent);
	}
}

void ARougeDemoPlayerController::OnLevelUp()
{
	CreateLevelUpUI();
}

void ARougeDemoPlayerController::UpdateLevelBar(float Percent, int32 Level)
{
	UpdateCharacterUI(Percent, Level);
}

void ARougeDemoPlayerController::LevelUpMaxHealth(bool PowerUp)
{
	
}

void ARougeDemoPlayerController::LevelUpMaxSpeed(bool PowerUp)
{
}

void ARougeDemoPlayerController::LevelUpTimerReduction(bool PowerUp)
{
}

void ARougeDemoPlayerController::LevelUpAbilityDamage(bool PowerUp)
{
}

void ARougeDemoPlayerController::SetupPlayer()
{
	//检查是否是本地
	if(IsLocalPlayerController())
	{
		ARougeDemoCharacter* RougeDemoCharacter = Cast<ARougeDemoCharacter>(GetPawn());
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

void ARougeDemoPlayerController::SetReference()
{
	
}

void ARougeDemoPlayerController::CreateLevelUpUI()
{
	if(HasAuthority())
	{
		bPreparingUI = true;
		bLevelUpHudUp = true;
		SetShowMouseCursor(true);
		PrepareLevelUp();
	}
}

void ARougeDemoPlayerController::PrepareLevelUp()
{
	if(LevelMasterWidgetClass == nullptr) { return; } 
	//创建LevelUpUI
	if(!LevelMasterWidget)
	{
		LevelMasterWidget = CreateWidget<ULevelMasterWidget>(this, LevelMasterWidgetClass);
		//添加卡片
		ExecuteLevelUp();
		//Bind Event to On Ready
		LevelMasterWidget->OnReadyDelegate.BindUObject(this, &ARougeDemoPlayerController::ExecuteLevelUp);
		//ProcessLevelup Bind to OnClose 处理点击选择后
		LevelMasterWidget->OnCloseDelegate.BindUObject(this, &ARougeDemoPlayerController::ProcessLevelUp);
	}else
	{
		LevelMasterWidget->ResetUI();
	}
	LevelMasterWidget->AddToViewport();
	//只能UI
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, LevelMasterWidget);
}

void ARougeDemoPlayerController::ExecuteLevelUp()
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
			CONTROLLER_LOG(Warning, TEXT("SkillLevelText[%s]"), *SkillLevelText.ToString());

			//相等的话，获取表中数据
			if(SkillLevelText.EqualTo(RowNameText))
			{
				FAbilityLevelUp* AbilityLevelUp = DT_ActiveAbilities->FindRow<FAbilityLevelUp>(RowName, "");
				if(AbilityLevelUp)
				{
					FString NameStr = UEnum::GetValueAsString(Local_EvoAbility);
					LevelMasterWidget->AddSelection(
						FText::FromString(NameStr),
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
		// if(CanAddActiveAbility)
		// {
		// 	CreateActiveCard(
		// 		Local_MaxCount,
		// 		Local_AvailableActiveAbilities,
		// 		Local_ActiveAbilitiesMap
		// 	);
		// }
		
		if(UKismetMathLibrary::RandomBoolWithWeight(0.5))
		{
			if(CanAddActiveAbility)
			{
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

bool ARougeDemoPlayerController::CheckIfEVOReady(EActiveAbilities& Ability)
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

TArray<EActiveAbilities> ARougeDemoPlayerController::CheckActiveAbilities(
	TMap<EActiveAbilities, int32> ActiveMap, int32 MaxLevel)
{
	int32 Local_MaxAbilityLevel = MaxLevel;
	TMap<EActiveAbilities, int32> Local_ActiveMap = ActiveMap;

	TArray<EActiveAbilities> LocalActiveAbilitiesArr;
	//已经激活的主动技能Map数量小于Card的数量
	if(LocalActiveAbilitiesArr.Num() < 4)
	{
		/* 用遍历枚举添加
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EActiveAbilities"), true);
		for(int i  = 0; i < EnumPtr->GetMaxEnumValue(); ++i)
		{
			LocalActiveAbilitiesArr.Add();
		}*/
		LocalActiveAbilitiesArr.Add(EActiveAbilities::EAA_Fireball);
		LocalActiveAbilitiesArr.Add(EActiveAbilities::EAA_Hammer);
		LocalActiveAbilitiesArr.Add(EActiveAbilities::EAA_Lightning);
		LocalActiveAbilitiesArr.Add(EActiveAbilities::EAA_FrostBolt);

		//删除已经满级的技能
		//Ex: 这里的满级是5，当某个主动技能5级时，则删除代表Card中不会再出现
		TArray<int32> OutValues;
		Local_ActiveMap.GenerateValueArray(OutValues);
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
				CONTROLLER_LOG(Warning, TEXT("OutKeys[Value][%s]"), *EnumString)
				LocalActiveAbilitiesArr.Add(OutKeys[Value]);
			}	
		}
	}
	return LocalActiveAbilitiesArr;
}

TArray<EPassiveAbilities> ARougeDemoPlayerController::CheckPassiveAbilities(TMap<EPassiveAbilities, int32> PassiveMap,
	int32 MaxLevel)
{
	int32 Local_MaxAbilityLevel = MaxLevel;
	TMap<EPassiveAbilities, int32> Local_PassiveMap = PassiveMap;

	TArray<EPassiveAbilities> LocalPassiveAbilitiesArr;
	//小于Card的数量
	if(Local_PassiveMap.Num() < 4)
	{
		//用遍历枚举添加
		// const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EPassiveAbilities"), true);
		// for(int i  = 0; i < EnumPtr->NumEnums(); ++i)
		// {
		// 	LocalPassiveAbilitiesArr.Add(*EnumPtr);
		// }
 		LocalPassiveAbilitiesArr.Add(EPassiveAbilities::EPA_Health);
		LocalPassiveAbilitiesArr.Add(EPassiveAbilities::EPA_Speed);
		LocalPassiveAbilitiesArr.Add(EPassiveAbilities::EPA_AbilityDamage);
		LocalPassiveAbilitiesArr.Add(EPassiveAbilities::EPA_CooldownReduction);

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

void ARougeDemoPlayerController::UpdateCharacterUI(float Percent, int32 Level)
{
	RougeDemoHUD = RougeDemoHUD == nullptr ? Cast<ARougeDemoHUD>(GetHUD()) : RougeDemoHUD;

	if(RougeDemoHUD->PlayerOverlayWidget)
	{
		RougeDemoHUD->PlayerOverlayWidget->PB_XP->SetPercent(Percent);
		//const FString HealthText = FString::Printf(TEXT("%d/%d"),FMath::CeilToInt(Health),FMath::CeilToInt(MaxHealth));
		FText LevelText = FText::Format(FText::FromString(TEXT("Level: {0}")), Level);
		RougeDemoHUD->PlayerOverlayWidget->TB_Level->SetText(LevelText);
	}
}

void ARougeDemoPlayerController::TestAction()
{
	RougeDemoHUD = RougeDemoHUD == nullptr ? Cast<ARougeDemoHUD>(GetHUD()) : RougeDemoHUD;

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

void ARougeDemoPlayerController::UpdateTime(FText Time)
{
	RougeDemoHUD = RougeDemoHUD == nullptr ? Cast<ARougeDemoHUD>(GetHUD()) : RougeDemoHUD;
	if(RougeDemoHUD->PlayerOverlayWidget)
	{
		RougeDemoHUD->PlayerOverlayWidget->UpdateTime(Time);
	}
}

void ARougeDemoPlayerController::CreateActiveCard(int32 Local_MaxCount, TArray<EActiveAbilities> Local_AvailableActiveAbilities,
	TMap<EActiveAbilities, int32> Local_ActiveAbilitiesMap)
{
	//从[0-3]随机选择一个数
	EActiveAbilities Local_ActiveAbility;
	int32 RandomIndex = UKismetMathLibrary::RandomInteger(2);
	
	//随机选择一个随机技能 
    Local_ActiveAbility = Local_AvailableActiveAbilities[RandomIndex];
	//从本地数组中删除，这样我们就没有重复Card了
   	Local_AvailableActiveAbilities.Remove(Local_ActiveAbility);
 	CanAddActiveAbility = !Local_AvailableActiveAbilities.IsEmpty();

	//从数据表中获取详细信息并创建升级卡
	TArray<FName> OutRowNames;
	UDataTableFunctionLibrary::GetDataTableRowNames(DT_ActiveAbilities, OutRowNames);
	
	for(FName RowName : OutRowNames)
	{
		FText RowNameText = UKismetTextLibrary::Conv_NameToText(RowName);
		
		//设置Level
		int32 Level = 0;
		if(Local_ActiveAbilitiesMap.Contains(Local_ActiveAbility))
		{
			Level = Local_ActiveAbilitiesMap[Local_ActiveAbility];
		}
		++Level;
		
		FString EvoAbilityStr = UEnum::GetValueAsString(Local_ActiveAbility);
		int32 Index_Local_ActiveAbility = EvoAbilityStr.Find("_");
		EvoAbilityStr = UKismetStringLibrary::GetSubstring(EvoAbilityStr, Index_Local_ActiveAbility + 1,EvoAbilityStr.Len());
		const FText EvoAbilityText = UKismetTextLibrary::Conv_StringToText(EvoAbilityStr);
		FText LevelText = FText::FromString(FString::FromInt(Level));
		FText SkillLevelText = FText::Format(FText::FromString(TEXT("{0}{1}")), EvoAbilityText, LevelText);
		CONTROLLER_LOG(Warning, TEXT("SkillLevelText[%s]"), *SkillLevelText.ToString());
		CONTROLLER_LOG(Warning, TEXT("RowNameText[%s]"), *RowNameText.ToString());
		//检查升级的卡片是否和表中的相等
		if(SkillLevelText.EqualTo(RowNameText))
		{
			FAbilityLevelUp* AbilityLevelUp = DT_ActiveAbilities->FindRow<FAbilityLevelUp>(RowName, "");
			if(AbilityLevelUp)
			{
				FString NameStr = UEnum::GetValueAsString(Local_ActiveAbility);
				//添加到UI
				LevelMasterWidget->AddSelection(
					FText::FromString(NameStr),
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

void ARougeDemoPlayerController::CreatePassiveCard(int32 Local_MaxCount,
	TArray<EPassiveAbilities> Local_AvailablePassiveAbilities,TMap<EPassiveAbilities, int32> Local_PassiveAbilitiesMap)
{
	//从本地数组中删除，这样我们就没有重复项了
	EPassiveAbilities Local_PassiveAbility;
	int32 RandomIndex = UKismetMathLibrary::RandomInteger(Local_MaxCount);
				
	Local_PassiveAbility = Local_AvailablePassiveAbilities[RandomIndex];
	Local_AvailablePassiveAbilities.Remove(Local_PassiveAbility);
	CanAddPassiveAbility = !Local_AvailablePassiveAbilities.IsEmpty();

	//检查升级的技能是否和表中的相等
	TArray<FName> OutRowNames;
	UDataTableFunctionLibrary::GetDataTableRowNames(DT_PassiveAbilities, OutRowNames);
	CONTROLLER_LOG(Warning, TEXT("OutRowNames.Num[%d]"), OutRowNames.Num());

	for(FName RowName : OutRowNames)
	{
		FText RowNameText = UKismetTextLibrary::Conv_NameToText(RowName);
		CONTROLLER_LOG(Warning, TEXT("RowNameText[%s]"), *RowNameText.ToString());
		
		//设置Level
		int32 Level = 0;
		if(Local_PassiveAbilitiesMap.Contains(Local_PassiveAbility))
		{
			Level = Local_PassiveAbilitiesMap[Local_PassiveAbility];
		}
		++Level;
					
		FString EvoAbilityStr = UEnum::GetValueAsString(Local_PassiveAbility);
		CONTROLLER_LOG(Warning, TEXT("EvoAbilityStr[%s]"), *EvoAbilityStr);
		//?
		int32 Index_Local_PassiveAbility = EvoAbilityStr.Find("_");
		EvoAbilityStr = UKismetStringLibrary::GetSubstring(EvoAbilityStr, Index_Local_PassiveAbility + 1,EvoAbilityStr.Len());
		const FText EvoAbilityText = UKismetTextLibrary::Conv_StringToText(EvoAbilityStr);
		const FText LevelText = FText::FromString(FString::FromInt(Level));
		FText SkillLevelText = FText::Format(FText::FromString(TEXT("{0}{1}")), EvoAbilityText, Level);
		CONTROLLER_LOG(Warning, TEXT("SkillLevelText[%s]"), *SkillLevelText.ToString());
		//相等的话，获取表中数据
		if(SkillLevelText.EqualTo(RowNameText))
		{
			FAbilityLevelUp* AbilityLevelUp = DT_PassiveAbilities->FindRow<FAbilityLevelUp>(RowName, "");

			if(AbilityLevelUp)
			{
				FString NameStr = UEnum::GetValueAsString(Local_PassiveAbility);
				//添加到UI
				LevelMasterWidget->AddSelection(
					FText::FromString(NameStr),
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

void ARougeDemoPlayerController::ProcessLevelUp(EAbilityType Type, EActiveAbilities ActiveAbilities, EPassiveAbilities PassiveAbilities)
{
	FString EnumName = UEnum::GetValueAsString(Type);
	UE_LOG(LogTemp, Warning, TEXT("EAbilityType[%s]"), *EnumName);

	bLevelUpHudUp = false;
	//是否触发了其他UI
	
	bPreparingUI = false;

	//暂停
	ARougeDemoGameMode* GameMode = Cast<ARougeDemoGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if(GameMode)
	{
		if (IGameModeInterface* GameModeImpl = Cast<IGameModeInterface>(GameMode))
		{
			GameModeImpl->Pause(false,false);
		}
	}
	
	SetShowMouseCursor(false);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
	AssignAbility(Type, ActiveAbilities, PassiveAbilities);

	//更新UI
	UpdateHotbar();
	
	//RefreshAbilities();
	AbilityComponent->RefreshAbilities();
}

void ARougeDemoPlayerController::AssignAbility(EAbilityType Type, EActiveAbilities ActiveAbilities,
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
				case EActiveAbilities::EAA_Fireball:
					break;
				case EActiveAbilities::EAA_Lightning:
					break;
				case EActiveAbilities::EAA_FrostBolt:
					break;
			}
			break;
		case EAbilityType::EAT_Passive:
			switch (PassiveAbilities)
			{
				case EPassiveAbilities::EPA_Health:
					
					break;
				case EPassiveAbilities::EPA_Speed:
					AbilityComponent->LevelUpHammer();
					break;
				case EPassiveAbilities::EPA_AbilityDamage:
					break;
				case EPassiveAbilities::EPA_CooldownReduction:
					break;
			}
			break;
		case EAbilityType::EAT_Evolution:
			break;
		case EAbilityType::EAT_Health:
			break;
		case EAbilityType::EAT_Gold:
			break;
	}
}

void ARougeDemoPlayerController::UpdateHudHotbar(TMap<EActiveAbilities, int32> ActiveMap,
	TMap<EPassiveAbilities, int32> PassiveMap)
{
	RougeDemoHUD = RougeDemoHUD == nullptr ? Cast<ARougeDemoHUD>(GetHUD()) : RougeDemoHUD;
	if(RougeDemoHUD->PlayerOverlayWidget)
	{
		RougeDemoHUD->PlayerOverlayWidget->BuildHotbar(ActiveMap, PassiveMap);
	}
}

void ARougeDemoPlayerController::UpdateHotbar()
{
	if(AbilityComponent == nullptr) { return; }
	UpdateHudHotbar(
		AbilityComponent->ActiveAbilitiesMap,
		AbilityComponent->PassiveAbilitiesMap
	);
}


