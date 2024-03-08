// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/AnimNotifies/AnimNotifyState_CanCombo.h"

#include "AbilitySystem/RougeAbilitySystemComponent.h"
#include "AbilitySystem/RougeGameplayEffectContext.h"
#include "Character/RougeCharacter.h"

void UAnimNotifyState_CanCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                             float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	/*ARougeCharacter* RougeCharacter = Cast<ARougeCharacter>(MeshComp->GetOwner());
	check(RougeCharacter);
	
	URougeAbilitySystemComponent* RougeASC = RougeCharacter->GetRougeAbilitySystemComponent();
	check(RougeASC);
	
	const FGameplayEffectContextHandle GameplayEffectContextHandle = {};
	RougeASC->BP_ApplyGameplayEffectToSelf(GameplayEffectClass, 0.f, GameplayEffectContextHandle);*/
	
}
void UAnimNotifyState_CanCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	/*ARougeCharacter* RougeCharacter = Cast<ARougeCharacter>(MeshComp->GetOwner());
	check(RougeCharacter);
	
	URougeAbilitySystemComponent* RougeASC = RougeCharacter->GetRougeAbilitySystemComponent();
	check(RougeASC);

	FGameplayEffectContextHandle GameplayEffectContextHandle = {};
	RougeASC->RemoveActiveGameplayEffectBySourceEffect(GameplayEffectClass, RougeASC);*/
}