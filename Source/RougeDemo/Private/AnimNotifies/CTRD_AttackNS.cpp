// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/CTRD_AttackNS.h"

#include "AI/BaseAI.h"
#include "Components/SphereComponent.h"

void UCTRD_AttackNS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	ABaseAI* BaseAI = Cast<ABaseAI>(MeshComp->GetOwner());
	if(BaseAI)
	{
		USphereComponent* AttackSphere = Cast<USphereComponent>(BaseAI->GetComponentByClass(USphereComponent::StaticClass()));
		if(AttackSphere)
		{
			AttackSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
	}
}

void UCTRD_AttackNS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	ABaseAI* BaseAI = Cast<ABaseAI>(MeshComp->GetOwner());
	if(BaseAI)
	{
		USphereComponent* AttackSphere = Cast<USphereComponent>(BaseAI->GetComponentByClass(USphereComponent::StaticClass()));
		if(AttackSphere)
		{
			AttackSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}
