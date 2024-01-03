// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AbilityComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Lib/RougeDemoFunctionLibary.h"
#include "SaveGame/PlayerSaveGame.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values for this component's properties
UAbilityComponent::UAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAbilityComponent::SetStartingAbility()
{
	//开始添加一个Hammer skill
	UPlayerSaveGame* PlayerSaveGame = URougeDemoFunctionLibary::LoadPlayerData();
	if(PlayerSaveGame)
	{
		EActiveAbilities ActiveAbilities = PlayerSaveGame->AvailableCharacter.ActiveAbilities;
		switch (ActiveAbilities)
		{
		case EActiveAbilities::EAA_Hammer:
			LevelUpHammer();
			break;
		}
	}
}

void UAbilityComponent::LevelUpHammer()
{
	int32 Local_Level = 1;
	if(ActiveAbilitiesMap.Contains(EActiveAbilities::EAA_Hammer))
	{
		Local_Level = *ActiveAbilitiesMap.Find(EActiveAbilities::EAA_Hammer);
		++Local_Level;
		ActiveAbilitiesMap.Add(EActiveAbilities::EAA_Hammer, Local_Level);
	}else
	{
		ActiveAbilitiesMap.Add(EActiveAbilities::EAA_Hammer, Local_Level);
	}
	
	switch (Local_Level)
	{
	case 1:
		GrantHammer(true);
		break;
	case 2:
		HammerDamage += 5;
		break;
	case 3:
		HammerDamage += 5;
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("LevelUpHammer Error"));
	}
}

void UAbilityComponent::GrantHammer(bool Cast)
{
	PrepareHammer();

	/*GetWorld()->GetTimerManager().SetTimer(
		PrepareHammerTimerHandle,
		this,
		&UAbilityComponent::PrepareHammerTimerHandleCallback,
		CalculateHammerCoolDown(),
		true
	);*/
	//唯一添加
	ActiveTimerArr.AddUnique(PrepareHammerTimerHandle);
}

void UAbilityComponent::PrepareHammer()
{
	TArray<FHitResult> OutArray;
	TArray<AActor*> IgnoreActor;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeQueryArr;
	UKismetSystemLibrary::SphereTraceMultiForObjects(
		this,
		GetOwner()->GetActorLocation(),
		GetOwner()->GetActorLocation(),
		HammerRadius,
		ObjectTypeQueryArr,
		false,
		IgnoreActor,
		EDrawDebugTrace::None,
		OutArray,
		true
	);
	
	if(HammerFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			HammerFX,
			GetOwner()->GetRootComponent(),
			FName("None"),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
		    EAttachLocation::KeepRelativeOffset,
			true,
			true,
			ENCPoolMethod::None,
			true
		);
	}
}

void UAbilityComponent::PrepareHammerTimerHandleCallback()
{
	PrepareHammer();
}

float UAbilityComponent::CalculateHammerCoolDown()
{
	return 1.0f;
}

void UAbilityComponent::RefreshAbilities()
{
	ActiveTimerArr.Empty();
	TArray<EActiveAbilities> ActiveArr;
	ActiveAbilitiesMap.GetKeys(ActiveArr);
	for(auto ActiveAbility : ActiveArr)
	{
		switch (ActiveAbility)
		{
		case EActiveAbilities::EAA_Hammer:
			GrantHammer(false);
			break;
		case EActiveAbilities::EAA_Lightning:
			
			break;
		case EActiveAbilities::EAA_FrostBolt:
			
			break;
		case EActiveAbilities::EAA_Fireball:
			
			break;
		}
	}

	
}

