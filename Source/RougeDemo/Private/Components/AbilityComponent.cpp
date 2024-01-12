// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AbilityComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Lib/RougeDemoFunctionLibary.h"
#include "SaveGame/PlayerSaveGame.h"
#include "NiagaraFunctionLibrary.h"
#include "Actor/BaseExplosion.h"
#include "Character/RougeCharacter.h"
#include "Interface/CharacterInterface.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "RougeDemo/RougeDemo.h"
#include "Weapon/ProjectileBase.h"

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
		case EActiveAbilities::EAA_Lightning:
			LevelUpLightning();
			break;
		case EActiveAbilities::EAA_FrostBolt:
			LevelUpFrostBolt();
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
	TArray<FHitResult> OutHits;
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
		OutHits,
		true
	);
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ExecuteHammer(OutHits, HammerDamage, HammerRadius, PlayerController);

	//Apply Damage
	
}

void UAbilityComponent::ExecuteHammer(TArray<FHitResult> Hits, float Damage, float Radius,
	APlayerController* Controller)
{
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

void UAbilityComponent::PrepareLightning()
{
	ICharacterInterface* CharacterInterface = Cast<ICharacterInterface>(GetOwner());
	if(CharacterInterface)
	{
		TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypeQueryArr;
		const EObjectTypeQuery EnemyObjectTypeQuery = UEngineTypes::ConvertToObjectType(ECC_TRACE_ENEMY);
		ObjectTypeQueryArr.Add(EnemyObjectTypeQuery);
		TArray<AActor*> IgnoreActorArr;
		TArray<AActor*> EnemyActorArr;
		//检查是否有Actor与AbilityComponent发送碰撞
		UKismetSystemLibrary::ComponentOverlapActors(
			CharacterInterface->GetAbilitySphere(),
			GetOwner()->GetTransform(),
			ObjectTypeQueryArr,
			nullptr,
			IgnoreActorArr,
			EnemyActorArr
		);

		if(EnemyActorArr.Num() > 0 && EnemyActorArr[0])
		{
			ARougeCharacter* Instigator = Cast<ARougeCharacter>(GetOwner());
			int32 RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, EnemyActorArr.Num() - 1);
			FVector TargetLocation = EnemyActorArr[RandomIndex]->GetActorLocation();
			LastLightningLoc = TargetLocation;
			
			ExecuteLightning(
				LastLightningLoc,
				Instigator,
				LightningDamage,
				LightningRadius
			);
		}
	}
	
}

void UAbilityComponent::PrepareHammerTimerHandleCallback()
{
	PrepareHammer();
}

void UAbilityComponent::PrepareLightningTimerHandleCallback()
{
	PrepareLightning();
}

float UAbilityComponent::CalculateHammerCoolDown()
{
	return 1.0f * AbilityCoolDownTimeMultiplier;
}

void UAbilityComponent::PrepareFrostBolt()
{
	FrostBoltIndex = 0;
	ICharacterInterface* CharacterImpl = Cast<ICharacterInterface>(GetOwner());
	if(CharacterImpl)
	{
		TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypeQueryArr;
		const EObjectTypeQuery EnemyObjectTypeQuery = UEngineTypes::ConvertToObjectType(ECC_TRACE_ENEMY);
		ObjectTypeQueryArr.Add(EnemyObjectTypeQuery);
		
		TArray<AActor*> IgnoreActorArr;
		TArray<AActor*> EnemyActorArr;
		//检查是否有Actor与AbilityComponent发送碰撞
		UKismetSystemLibrary::ComponentOverlapActors(
			CharacterImpl->GetAbilitySphere(),
			GetOwner()->GetTransform(),
			ObjectTypeQueryArr,
			nullptr,
			IgnoreActorArr,
			EnemyActorArr
		);

		if(EnemyActorArr.Num() > 0 && EnemyActorArr[0])
		{
			ARougeCharacter* Character = Cast<ARougeCharacter>(GetOwner());
			FVector Origin = GetOwner()->GetActorLocation();
			float Distance = 0.f;
			AActor* Target = UGameplayStatics::FindNearestActor(
				Origin,
				EnemyActorArr,
				Distance
			);

			ExecuteFrostBolt(Target, Character, FrostBoltDamage);
		}
	}
}

void UAbilityComponent::ExecuteFrostBolt(AActor* TargetActor, ACharacter* Instigator, float Damage)
{
	FRotator LookDirection = UKismetMathLibrary::FindLookAtRotation(
		Instigator->GetActorLocation(),
		TargetActor->GetActorLocation()
	);
	
	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	AProjectileBase* ProjectileBase = GetWorld()->SpawnActor<AProjectileBase>(
		FrostBoltClass,
		Instigator->GetActorLocation(),
		FRotator(0.f,LookDirection.Yaw,LookDirection.Roll),
		ActorSpawnParameters
	);
	if(ProjectileBase && HitFX)
	{
		ProjectileBase->Setup(
			HitFX,
			Damage
		);
	}
}

void UAbilityComponent::PrepareFrostBoltTimerHandleCallback()
{
	PrepareFrostBolt();
}

void UAbilityComponent::GrantFrostBolt(bool Cast)
{
	GetWorld()->GetTimerManager().SetTimer(
		PrepareFrostBoltTimerHandle,
		this,
		&UAbilityComponent::PrepareFrostBoltTimerHandleCallback,
		2.0f,
		true,
		-1
	);
	
	ActiveTimerArr.AddUnique(PrepareFrostBoltTimerHandle);
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
			GrantLightning(false);
			break;
		case EActiveAbilities::EAA_FrostBolt:
			GrantFrostBolt(false);
			break;
		case EActiveAbilities::EAA_FireBall:
			
			break;
		}
	}

	
}

void UAbilityComponent::LevelUpLightning()
{
	int32 Local_Level = 1;
	if(ActiveAbilitiesMap.Contains(EActiveAbilities::EAA_Lightning))
	{
		Local_Level = *ActiveAbilitiesMap.Find(EActiveAbilities::EAA_Lightning);
		++Local_Level;
		ActiveAbilitiesMap.Add(EActiveAbilities::EAA_Lightning, Local_Level);
	}else
	{
		ActiveAbilitiesMap.Add(EActiveAbilities::EAA_Lightning, Local_Level);
	}
	
	switch (Local_Level)
	{
	case 1:
		GrantLightning(true);
		break;
	case 2:
		LightningDamage += 5;
		break;
	case 3:
		LightningDamage += 5;
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("LevelUpLightning Error"));
	}
}

void UAbilityComponent::LevelUpFrostBolt()
{
	int32 Local_Level = 1;
	if(ActiveAbilitiesMap.Contains(EActiveAbilities::EAA_FrostBolt))
	{
		Local_Level = *ActiveAbilitiesMap.Find(EActiveAbilities::EAA_FrostBolt);
		++Local_Level;
		ActiveAbilitiesMap.Add(EActiveAbilities::EAA_FrostBolt, Local_Level);
	}else
	{
		ActiveAbilitiesMap.Add(EActiveAbilities::EAA_FrostBolt, Local_Level);
	}
	
	switch (Local_Level)
	{
	case 1:
		GrantFrostBolt(true);
		break;
	case 2:
		FrostBoltDamage += 5;
		break;
	case 3:
		FrostBoltDamage += 5;
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("LevelUpFrostBolt Error"));
	}
}


void UAbilityComponent::ExecuteLightning(FVector TargetLocation, ACharacter* Instigator, float Damage,
                                         float Radius)
{
	if(LightningClass)
	{
		ABaseExplosion* BaseExplosion = GetWorld()->SpawnActor<ABaseExplosion>(
			LightningClass,
			TargetLocation - FVector(0.f, 0.f, 100.f),
			FRotator::ZeroRotator
		);
		
		BaseExplosion->SetUp(
			Damage,
			Radius,
			LightningSystem
		);

		BaseExplosion->DoWork();
	}
}

void UAbilityComponent::GrantLightning(bool Case)
{
	GetWorld()->GetTimerManager().SetTimer(
		PrepareLightningTimerHandle,
		this,
		&UAbilityComponent::PrepareLightningTimerHandleCallback,
		2.0f,
		true
	);
	
	ActiveTimerArr.AddUnique(PrepareLightningTimerHandle);
}

void UAbilityComponent::LevelUpMaxHealth(bool PowerUp)
{
	int32 Local_Level = 1;
	ICharacterInterface* CharacterImpl = Cast<ICharacterInterface>(GetOwner());
	if(PowerUp)
	{
		if(CharacterImpl)
		{
			CharacterImpl->AdjustPassive(EPassiveAbilities::EPA_Health, 1.1f);
		}
	}else
	{
		if(PassiveAbilitiesMap.Contains(EPassiveAbilities::EPA_Health))
		{
			Local_Level = *PassiveAbilitiesMap.Find(EPassiveAbilities::EPA_Health);
			++Local_Level;
			PassiveAbilitiesMap.Add(EPassiveAbilities::EPA_Health, Local_Level);
		}else
		{
			PassiveAbilitiesMap.Add(EPassiveAbilities::EPA_Health, Local_Level);
		}
		
		switch (Local_Level)
		{
		case 1:
		case 2:
		case 3:
		case 4:
			if(CharacterImpl)
			{
				CharacterImpl->AdjustPassive(EPassiveAbilities::EPA_Health, 1.1f);
			}
			break;
		case 5:
			if(CharacterImpl)
			{
				CharacterImpl->AdjustPassive(EPassiveAbilities::EPA_Health, 1.1f);
			}
			EvolutionPassiveArr.AddUnique(EPassiveAbilities::EPA_Health);
			break;
		}
	}
}

void UAbilityComponent::LevelUpTimerReduction(bool PowerUp)
{
	int32 Local_Level = 1;
	if(PowerUp)
	{
		AbilityCoolDownTimeMultiplier = AbilityCoolDownTimeMultiplier * 1.1;
	}else
	{
		if(PassiveAbilitiesMap.Contains(EPassiveAbilities::EPA_CooldownReduction))
		{
			Local_Level = *PassiveAbilitiesMap.Find(EPassiveAbilities::EPA_CooldownReduction);
			++Local_Level;
			PassiveAbilitiesMap.Add(EPassiveAbilities::EPA_CooldownReduction, Local_Level);
		}else
		{
			PassiveAbilitiesMap.Add(EPassiveAbilities::EPA_CooldownReduction, Local_Level);
		}

		switch (Local_Level)
		{
		case 1:
		case 2:
		case 3:
		case 4:
			AbilityCoolDownTimeMultiplier = AbilityCoolDownTimeMultiplier * 1.1;
			break;
		case 5:
			AbilityCoolDownTimeMultiplier = AbilityCoolDownTimeMultiplier * 1.1;
			EvolutionPassiveArr.AddUnique(EPassiveAbilities::EPA_CooldownReduction);
		}
	}
}

void UAbilityComponent::LevelUpWalkSpeed(bool PowerUp)
{
	int32 Local_Level = 1;
	ICharacterInterface* CharacterImpl = Cast<ICharacterInterface>(GetOwner());
	if(PowerUp)
	{
		if(CharacterImpl)
		{
			CharacterImpl->AdjustPassive(EPassiveAbilities::EPA_Speed, 1.1f);
		}
	}else
	{
		if(PassiveAbilitiesMap.Contains(EPassiveAbilities::EPA_Speed))
		{
			Local_Level = *PassiveAbilitiesMap.Find(EPassiveAbilities::EPA_Speed);
			++Local_Level;
			PassiveAbilitiesMap.Add(EPassiveAbilities::EPA_Speed, Local_Level);
		}else
		{
			PassiveAbilitiesMap.Add(EPassiveAbilities::EPA_Speed, Local_Level);
		}
		
		switch (Local_Level)
		{
		case 1:
		case 2:
		case 3:
		case 4:
			if(CharacterImpl)
			{
				CharacterImpl->AdjustPassive(EPassiveAbilities::EPA_Speed, 1.1f);
			}
			break;
		case 5:
			if(CharacterImpl)
			{
				CharacterImpl->AdjustPassive(EPassiveAbilities::EPA_Speed, 1.1f);
			}
			EvolutionPassiveArr.AddUnique(EPassiveAbilities::EPA_Speed);
			break;
		}
	}
}

void UAbilityComponent::LevelUpAbilityDamage(bool PowerUp)
{
	int32 Local_Level = 1;
	if(PowerUp)
	{
		AbilityDamageMultiplier = AbilityDamageMultiplier * 1.1;
	}else
	{
		if(PassiveAbilitiesMap.Contains(EPassiveAbilities::EPA_AbilityDamage))
		{
			Local_Level = *PassiveAbilitiesMap.Find(EPassiveAbilities::EPA_AbilityDamage);
			++Local_Level;
			PassiveAbilitiesMap.Add(EPassiveAbilities::EPA_AbilityDamage, Local_Level);
		}else
		{
			PassiveAbilitiesMap.Add(EPassiveAbilities::EPA_AbilityDamage, Local_Level);
		}

		switch (Local_Level)
		{
		case 1:
		case 2:
		case 3:
		case 4:
			AbilityDamageMultiplier = AbilityDamageMultiplier * 1.1;
			break;
		case 5:
			AbilityDamageMultiplier = AbilityDamageMultiplier * 1.1;
			EvolutionPassiveArr.AddUnique(EPassiveAbilities::EPA_AbilityDamage);
		}
	}
}


