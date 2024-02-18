// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/RougeGameplayAbility_Hammer.h"

#include "NiagaraCommon.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/RougeCharacter.h"
#include "Kismet/GameplayStatics.h"

void URougeGameplayAbility_Hammer::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                   const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                   const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	PrepareHammer();
}

void URougeGameplayAbility_Hammer::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URougeGameplayAbility_Hammer::GrantHammer(bool Cast)
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
}

void URougeGameplayAbility_Hammer::PrepareHammer()
{
	ARougeCharacter* RougeCharacter = Cast<ARougeCharacter>(CurrentActorInfo->AvatarActor.Get());
	if(RougeCharacter == nullptr)
	{
		return;
	}
	
	TArray<FHitResult> OutHits;
	TArray<AActor*> IgnoreActor;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeQueryArr;
	UKismetSystemLibrary::SphereTraceMultiForObjects(
		this,
		RougeCharacter->GetActorLocation(),
		RougeCharacter->GetActorLocation(),
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
}

void URougeGameplayAbility_Hammer::ExecuteHammer(TArray<FHitResult> Hits, float Damage, float Radius,
	APlayerController* Controller)
{
	ARougeCharacter* RougeCharacter = Cast<ARougeCharacter>(CurrentActorInfo->AvatarActor.Get());
	if(RougeCharacter == nullptr)
	{
		return;
	}
	
	if(HammerFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			HammerFX,
			RougeCharacter->GetRootComponent(),
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

void URougeGameplayAbility_Hammer::PrepareHammerTimerHandleCallback()
{
}
