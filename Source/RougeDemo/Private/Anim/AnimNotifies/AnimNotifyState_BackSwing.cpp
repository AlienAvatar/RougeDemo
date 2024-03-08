// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/AnimNotifies/AnimNotifyState_BackSwing.h"

#include "AbilitySystem/RougeAbilitySystemComponent.h"
#include "Character/RougeCharacter.h"
#include "Core/RougePlayerState.h"

void UAnimNotifyState_BackSwing::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                             float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	/*check(GameplayEffectClass);
	
	ARougeCharacter* RougeCharacter = Cast<ARougeCharacter>(MeshComp->GetOwner());
	check(RougeCharacter);
	
	URougeAbilitySystemComponent* RougeASC = RougeCharacter->GetRougeAbilitySystemComponent();
	check(RougeASC);

	
	const FGameplayEffectContextHandle GameplayEffectContextHandle = {};
	RougeASC->BP_ApplyGameplayEffectToSelf(GameplayEffectClass, 0.f, GameplayEffectContextHandle);*/
	
}
void UAnimNotifyState_BackSwing::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	/*ARougeCharacter* RougeCharacter = Cast<ARougeCharacter>(MeshComp->GetOwner());
	check(RougeCharacter);
	
	URougeAbilitySystemComponent* RougeASC = RougeCharacter->GetRougeAbilitySystemComponent();
	check(RougeASC);

	FGameplayEffectContextHandle GameplayEffectContextHandle = {};
	RougeASC->RemoveActiveGameplayEffectBySourceEffect(GameplayEffectClass, RougeASC);*/
}
