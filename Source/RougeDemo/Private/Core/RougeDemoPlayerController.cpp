// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/RougeDemoPlayerController.h"

#include "Character/RougeDemoCharacter.h"
#include "Components/AbilityComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Core/GameManager.h"
#include "HUD/PlayerOverlayWidget.h"
#include "HUD/RougeDemoHUD.h"
#include "HUD/Game/LevelMasterWidget.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "Lib/RougeDemoFunctionLibary.h"
#include "Struct/AbilityLevelUp.h"

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

void ARougeDemoPlayerController::SetupPlayer()
{
	//检查是否是本地
	if(IsLocalPlayerController())
	{
		ARougeDemoCharacter* RougeDemoCharacter = Cast<ARougeDemoCharacter>(GetPawn());
		AbilityComponent = RougeDemoCharacter->GetAbilityComponent();
	}
}

void ARougeDemoPlayerController::SetReference()
{
	
}

void ARougeDemoPlayerController::CreateLevelUpUI()
{
	if(HasAuthority())
	{
		PrepareLevelUp();
	}
}

void ARougeDemoPlayerController::PrepareLevelUp()
{
	//创建LevelUpUI
	if(!LevelMasterWidget)
	{
		LevelMasterWidget = CreateWidget<ULevelMasterWidget>(this, ULevelMasterWidget::StaticClass());
		ExecuteLevelUp();
	}else
	{
		LevelMasterWidget->ResetUI();
	}
	LevelMasterWidget->AddToViewport();
	
}

void ARougeDemoPlayerController::ExecuteLevelUp()
{
	if(AbilityComponent == nullptr) { return; }

	//保存为本地变量
	TMap<EActiveAbilities, int32> Local_ActiveAbilitiesMap = AbilityComponent->ActiveAbilitiesMap;
	TMap<EPassiveAbilities, int32> Local_PassiveAbilitiesMap = AbilityComponent->PassiveAbilitiesMap;
	int32 Local_MaxAbilityLevel = AbilityComponent->MaxAbilityLevel;

	//检查是否有可以升级的技能
	EActiveAbilities Local_EvoAbility;
	bool bLocal_EvoReady = CheckIfEVOReady(Local_EvoAbility);

	//设置最大的展示Card
	int32 Local_MaxCount = 4;

	int Local_CardCount = 0;
	//如果Local_EvoReady = true， 就升级技能
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
			FText SkillLevelText = FText::Format(FText::FromString(TEXT("{skill}{level}")), EvoAbilityText, UKismetSystemLibrary::MakeLiteralInt(-1));

			//相等的话，获取表中数据
			if(SkillLevelText.EqualTo(RowNameText))
			{
				FAbilityLevelUp* AbilityLevelUp = DT_ActiveAbilities->FindRow<FAbilityLevelUp>(RowName, "");
				if(AbilityLevelUp)
				{
					AbilityLevelUp->LevelUpText;
					//LevelMasterWidget->AddSelection()
				}
			}
		}
	}
	//获得其他技能，防止某个技能重复出现（刷爆）
	//检查主动技能
	bool bLocal_CanAddActiveAbility = true;
	TArray<EActiveAbilities> Local_AvailableActiveAbilities = CheckActiveAbilities(Local_ActiveAbilitiesMap, Local_MaxAbilityLevel);
	if(Local_AvailableActiveAbilities.IsEmpty())
	{
		bLocal_CanAddActiveAbility = false;
	}
	TArray<int32> Local_ActiveRandomArr;
	const UEnum* ActiveEnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EActiveAbilities"), true);
	for(int i = 0; i < Local_AvailableActiveAbilities.Num(); ++i)
	{
		Local_ActiveRandomArr.Add(ActiveEnumPtr->GetValueByIndex(i));
	}
	
	//检查被动技能
	bool bLocal_CanAddPassiveAbility = true;
	TArray<EPassiveAbilities> Local_AvailablePassiveAbilities = CheckPassiveAbilities(Local_PassiveAbilitiesMap, Local_MaxAbilityLevel);
	if(Local_AvailablePassiveAbilities.IsEmpty())
	{
		bLocal_CanAddPassiveAbility = false;
	}
	TArray<int32> Local_PassiveRandomArr;
	const UEnum* PassiveEnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EPassiveAbilities"), true);
	for(int i = 0; i < Local_AvailablePassiveAbilities.Num(); ++i)
	{
		Local_PassiveRandomArr.Add(PassiveEnumPtr->GetValueByIndex(i));
	}
	
	//运行循环，直到我们打出我们想要的卡牌或不能添加被动/主动技能
	while((Local_CardCount < Local_MaxCount) && (bLocal_CanAddActiveAbility || bLocal_CanAddPassiveAbility))
	{
		++Local_CardCount;
		//确定主动或被动的随机bool，如果不允许，则为failsafe
		if(UKismetMathLibrary::RandomBoolWithWeight(0.5))
		{
			if(bLocal_CanAddActiveAbility)
			{
				//从本地数组中删除，这样我们就没有重复项了
				EActiveAbilities Local_ActiveAbility;
				/*int32 OutIndex;
				int32 RandomIndex;
				UKismetArrayLibrary::Array_Random(Local_ActiveRandomArr,RandomIndex, OutIndex);*/
				int32 RandomIndex = UKismetMathLibrary::RandomInteger(Local_MaxCount);
				
				Local_ActiveAbility = Local_AvailableActiveAbilities[RandomIndex];
				Local_AvailableActiveAbilities.Remove(Local_ActiveAbility);
				bLocal_CanAddActiveAbility = Local_AvailableActiveAbilities.IsEmpty();

				//检查升级的技能是否和表中的相等
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
					
					FString EvoAbilityStr = UEnum::GetValueAsString(Local_EvoAbility);
					const FText EvoAbilityText = UKismetTextLibrary::Conv_StringToText(EvoAbilityStr);
					FText SkillLevelText = FText::Format(FText::FromString(TEXT("{skill}{level}")), EvoAbilityText, Level);

					//相等的话，获取表中数据
					if(SkillLevelText.EqualTo(RowNameText))
					{
						FAbilityLevelUp* AbilityLevelUp = DT_ActiveAbilities->FindRow<FAbilityLevelUp>(RowName, "");
						if(AbilityLevelUp)
						{
							FString NameStr = UEnum::GetValueAsString(Local_ActiveAbility);
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
			}else
			{
				
			}
		}else
		{
			if(bLocal_CanAddPassiveAbility)
			{
				//从本地数组中删除，这样我们就没有重复项了
				EPassiveAbilities Local_PassiveAbility;
				/*int32 OutIndex;
				int32 RandomIndex;
				UKismetArrayLibrary::Array_Random(Local_PassiveRandomArr,RandomIndex, OutIndex);*/
				int32 RandomIndex = UKismetMathLibrary::RandomInteger(Local_MaxCount);
				
				Local_PassiveAbility = Local_AvailablePassiveAbilities[RandomIndex];
				Local_AvailablePassiveAbilities.Remove(Local_PassiveAbility);
				bLocal_CanAddPassiveAbility = Local_AvailablePassiveAbilities.IsEmpty();

				//检查升级的技能是否和表中的相等
				TArray<FName> OutRowNames;
				UDataTableFunctionLibrary::GetDataTableRowNames(DT_PassiveAbilities, OutRowNames);
				
				for(FName RowName : OutRowNames)
				{
					FText RowNameText = UKismetTextLibrary::Conv_NameToText(RowName);

					//设置Level
					int32 Level = 0;
					if(Local_PassiveAbilitiesMap.Contains(Local_PassiveAbility))
					{
						Level = Local_PassiveAbilitiesMap[Local_PassiveAbility];
					}
					++Level;
					
					FString EvoAbilityStr = UEnum::GetValueAsString(Local_EvoAbility);
					const FText EvoAbilityText = UKismetTextLibrary::Conv_StringToText(EvoAbilityStr);
					FText SkillLevelText = FText::Format(FText::FromString(TEXT("{skill}{level}")), EvoAbilityText, Level);

					//相等的话，获取表中数据
					if(SkillLevelText.EqualTo(RowNameText))
					{
						FAbilityLevelUp* AbilityLevelUp = DT_PassiveAbilities->FindRow<FAbilityLevelUp>(RowName, "");
						if(AbilityLevelUp)
						{
							FString NameStr = UEnum::GetValueAsString(Local_PassiveAbility);
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
	//小于Card的数量
	if(Local_ActiveMap.Num() < 4)
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

		//删除已经满的技能
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

		//返回任何尚未学习或已满的技能
		return LocalActiveAbilitiesArr;
	}else
	{
		//删除已经满的技能
		TArray<int32> OutValues;
		Local_ActiveMap.GenerateValueArray(OutValues);
		for(auto Value : OutValues)
		{
			if(Value >= Local_MaxAbilityLevel)
			{
				TArray<EActiveAbilities> OutKeys;
				Local_ActiveMap.GetKeys(OutKeys);

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
		/* 用遍历枚举添加
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EPassiveAbilities"), true);
		for(int i  = 0; i < EnumPtr->GetMaxEnumValue(); ++i)
		{
			LocalPassiveAbilitiesArr.Add();
		}*/
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
			if(Value >= Local_MaxAbilityLevel)
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
		FText LevelText = FText::Format(FText::FromString(TEXT("Level: {level}")), Level);
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
			Percent = Percent+0.8f;
			GameManager->UpdateCharactersXP(Percent, 2);
			if(Percent > 0.9)
			{
				Percent = 0;
				GameManager->PrepareLevelUp();
			}
		}
	}
}
