// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/RougeGameplayAbility_Lighting.h"

#include "AbilitySystem/RougeAbilitySystemComponent.h"
#include "Actor/BaseExplosion.h"
#include "Character/RougeCharacter.h"
#include "Components/SphereComponent.h"
#include "Interface/CharacterInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RougeDemo/RougeDemo.h"

void URougeGameplayAbility_Lighting::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	check(ActorInfo);

	
	URougeAbilitySystemComponent* RougeASC = CastChecked<URougeAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());
	PrepareLightning();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

}

void URougeGameplayAbility_Lighting::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URougeGameplayAbility_Lighting::GrantLightning(bool Case)
{
	GetWorld()->GetTimerManager().SetTimer(
		PrepareLightningTimerHandle,
		this,
		&URougeGameplayAbility_Lighting::PrepareLightningTimerHandleCallback,
		2.0f,
		true
	);
}

void URougeGameplayAbility_Lighting::PrepareLightningTimerHandleCallback()
{
	PrepareLightning();
}

void URougeGameplayAbility_Lighting::PrepareLightning()
{
	ARougeCharacter* RougeCharacter = Cast<ARougeCharacter>(CurrentActorInfo->AvatarActor.Get());
	if(RougeCharacter == nullptr)
	{
		return;
	}
	ICharacterInterface* CharacterInterface = Cast<ICharacterInterface>(RougeCharacter);
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
			RougeCharacter->GetTransform(),
			ObjectTypeQueryArr,
			nullptr,
			IgnoreActorArr,
			EnemyActorArr
		);
		// 是否有Enemy在其范围内
		if(EnemyActorArr.Num() > 0 && EnemyActorArr[0])
		{
			ARougeCharacter* Instigator = Cast<ARougeCharacter>(RougeCharacter);
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

void URougeGameplayAbility_Lighting::ExecuteLightning(FVector TargetLocation, ACharacter* Instigator, float Damage,
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
