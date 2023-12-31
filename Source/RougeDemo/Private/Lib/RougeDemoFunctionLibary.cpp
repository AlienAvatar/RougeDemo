// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/RougeDemoFunctionLibary.h"

#include "Core/RougeDemoInstance.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/PlayerSaveGame.h"

URougeDemoInstance* URougeDemoFunctionLibary::GetRougeDemoGameInstance(UObject* WorldContextObject)
{
	return Cast<URougeDemoInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
}

void URougeDemoFunctionLibary::PlayLoadingScreen(bool bPlayUntilStopped, float PlayTime)
{
	/*IRougeDemoLoadingScreenModule& LoadingScreenModule = IRougeDemoLoadingScreenModule::Get();
	LoadingScreenModule.StartInGameLoadingScreen(bPlayUntilStopped, PlayTime);*/
}

void URougeDemoFunctionLibary::StopLoadingScreen()
{
	/*IRougeDemoLoadingScreenModule& LoadingScreenModule = IRougeDemoLoadingScreenModule::Get();
	LoadingScreenModule.StopInGameLoadingScreen();*/
}

UTexture2D* URougeDemoFunctionLibary::FindActionIcon(EActiveAbilities AAbility)
{
	UTexture2D* Texture2D = nullptr;
	FString Path;
	
	switch (AAbility)
	{
	case EActiveAbilities::EAA_Hammer:
		Path = "/Game/RougeDemo/RES/UI/Texture/GamePanel/hammer-drop.hammer-drop";
		break;
	case EActiveAbilities::EAA_FireBall:
		Path = "/Game/RougeDemo/RES/UI/Texture/GamePanel/fireball.fireball";
		break;
	case EActiveAbilities::EAA_Lightning:
		Path = "/Game/RougeDemo/RES/UI/Texture/GamePanel/focused-lightning.focused-lightning";
		break;
	case EActiveAbilities::EAA_FrostBolt:
		Path = "/Game/RougeDemo/RES/UI/Texture/GamePanel/ice-spear.ice-spear";
		break;
	}
	if(Path.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Not exist the Texture in content brower"));
		return Texture2D;
	}
	
	Texture2D = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *Path));

	return Texture2D;
}

UTexture2D* URougeDemoFunctionLibary::FindPassiveIcon(EPassiveAbilities PAbility)
{
	UTexture2D* Texture2D = nullptr;
	FString Path;
	switch (PAbility)
	{
	case EPassiveAbilities::EPA_Health:
		Path = "/Game/RougeDemo/RES/UI/Texture/GamePanel/health-increase.health-increase";
		break;
	case EPassiveAbilities::EPA_Speed:
		Path = "/Game/RougeDemo/RES/UI/Texture/GamePanel/run.run";
		break;
	case EPassiveAbilities::EPA_AbilityDamage:
		Path = "/Game/RougeDemo/RES/UI/Texture/GamePanel/upgrade.upgrade";
		break;
	case EPassiveAbilities::EPA_CooldownReduction:
		Path = "/Game/RougeDemo/RES/UI/Texture/GamePanel/speedometer.speedometer";
		break;
	}
	if(Path.IsEmpty())
	{
		return Texture2D;
	}
	
	if(Path.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Texture2D not found"));
		return Texture2D;
	}
	
	Texture2D = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *Path));

	return Texture2D;
}

UPlayerSaveGame* URougeDemoFunctionLibary::LoadPlayerData()
{
	FString SlotName = "PlayerData1";
	bool bExist = UGameplayStatics::DoesSaveGameExist(SlotName, 0);
	if(bExist)
	{
		USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(SlotName, 0);
		return Cast<UPlayerSaveGame>(SaveGame);
	}else
	{
		USaveGame* SaveGame = UGameplayStatics::CreateSaveGameObject(UPlayerSaveGame::StaticClass());
		UPlayerSaveGame* PlayerSaveGame = Cast<UPlayerSaveGame>(SaveGame);
		if(PlayerSaveGame)
		{
			FString Path = "/Script/Engine.DataTable'/Game/RougeDemo/SRC/Data/DataTables/DT_AvaliableCharacter.DT_AvaliableCharacter'";
			UDataTable* Table =  Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *Path));
			
			if(Table)
			{
				FAvailableCharacter* AvailableCharacter = Table->FindRow<FAvailableCharacter>(FName("Char1"),"");
				if(AvailableCharacter)
				{
					PlayerSaveGame->AvailableCharacter = *AvailableCharacter;
				}
			}
			return PlayerSaveGame;
		}
		return Cast<UPlayerSaveGame>(SaveGame);
	}
}

void URougeDemoFunctionLibary::DamageEnemiesOnce(TArray<FHitResult> EnemyHits, float Damage,
	AController* InstigatorController, AActor* Causer)
{
	if(InstigatorController == nullptr || Causer == nullptr) { return; }
	TArray<AActor*> Local_HitActorArr;
	for(int i = 0; i < EnemyHits.Num() - 1; ++i)
	{
		FHitResult HitResult = EnemyHits[i];
		if(Local_HitActorArr.Contains(HitResult.GetActor()))
		{
			continue;
		}else
		{
			Local_HitActorArr.Add(HitResult.GetActor());
		}

		UGameplayStatics::ApplyDamage(
			HitResult.GetActor(),
			Damage,
			InstigatorController,
			Causer,
			nullptr
		);
		
	}
}
