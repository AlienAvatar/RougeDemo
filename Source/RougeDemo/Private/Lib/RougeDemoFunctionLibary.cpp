// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/RougeDemoFunctionLibary.h"

#include "Core/RougeDemoInstance.h"
#include "Kismet/GameplayStatics.h"

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
	case EActiveAbilities::EAA_Fireball:
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
		UE_LOG(LogTemp, Error, TEXT("Texture2D not found"));
		return Texture2D;
	}
	
	static ConstructorHelpers::FObjectFinder<UTexture2D> Texture(*Path);
	if (Texture.Succeeded())
	{
		Texture2D = Texture.Object;
	}
	return Texture2D;
}

UTexture2D* URougeDemoFunctionLibary::FindPassiveIcon(EPassiveAbilities PAbility)
{
	UTexture2D* Texture2D = nullptr;
	FString Path;
	switch (PAbility)
	{
	case EPassiveAbilities::EPA_Health:
		Path = "/Game/StarterContent/Textures/T_Spark_Core.T_Spark_Core";
		break;
	case EPassiveAbilities::EPA_Speed:
		Path = "/Game/StarterContent/Textures/T_Spark_Core.T_Spark_Core";
		break;
	case EPassiveAbilities::EPA_AbilityDamage:
		Path = "/Game/StarterContent/Textures/T_Spark_Core.T_Spark_Core";
		break;
	case EPassiveAbilities::EPA_CooldownReduction:
		Path = "/Game/StarterContent/Textures/T_Spark_Core.T_Spark_Core";
		break;
	}
	if(Path.IsEmpty())
	{
		return Texture2D;
	}
	
	static ConstructorHelpers::FObjectFinder<UTexture2D> Texture(*Path);
	if (Texture.Succeeded())
	{
		UE_LOG(LogTemp, Error, TEXT("Texture2D not found"));
		Texture2D = Texture.Object;
	}
	return Texture2D;
}
