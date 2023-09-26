// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/MeleeWeapon.h"

#include "AI/BaseAI.h"
#include "Character/PlayerKatanaCharacter.h"
#include "Character/RougeDemoAnimInstance.h"
#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Perception/AISense_Damage.h"


AMeleeWeapon::AMeleeWeapon()
{
	GetWeaponMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TrailComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail"));
	TrailComp->SetupAttachment(GetWeaponMesh());
}

void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();

	if(AttackBox)
	{
		AttackBox->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnBeginOverlap);
		AttackBox->OnComponentEndOverlap.AddDynamic(this, &AMeleeWeapon::OnEndOverlap);
	}
}

void AMeleeWeapon::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if(!bIsOverlaping)
	{
		bIsOverlaping = true;
		//UE_LOG(LogTemp,Warning,TEXT("OnBeginOverlap"));
		UE_LOG(LogTemp,Warning,TEXT("OverlappedComponent[%s]"),*OverlappedComponent->GetName());
		UE_LOG(LogTemp,Warning,TEXT("OtherComp[%s]"),*OtherComp->GetName());
		UE_LOG(LogTemp,Warning,TEXT("OtherActor[%s]"),*OtherActor->GetName());

		DrawDebugSphere(GetWorld(),SweepResult.Location,10.f,-1,FColor::Red);
		UE_LOG(LogTemp,Warning,TEXT("SweepResult.Location[%s]"),*SweepResult.Location.ToString());
		UE_LOG(LogTemp,Warning,TEXT("OtherBodyIndex[%d]"),OtherBodyIndex);
		
		ABaseAI* BaseAI = Cast<ABaseAI>(OtherActor);
		if(BaseAI)
		{
			//To Do 做伤害类型判断，根据不同的蒙太奇动画，选择不同的削韧和伤害
			
			FDamageEvent DamageEvent;
			APlayerKatanaCharacter* OwnerCharacter = Cast<APlayerKatanaCharacter>(GetOwner());
			if(OwnerCharacter)
			{
				AController* OwnerController = OwnerCharacter->Controller;
				if(OwnerController)
				{
					UE_LOG(LogTemp,Warning,TEXT("DamageCount[%f]"),DamageCount);
					UGameplayStatics::ApplyDamage(OtherActor,DamageCount, OwnerController,this,UDamageType::StaticClass());
				}
			}
		}
	}
}

void AMeleeWeapon::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//UE_LOG(LogTemp,Warning,TEXT("OnEndOverlap"));
	bIsOverlaping = false;
	/*ABaseAI* BaseAI = Cast<ABaseAI>(OtherActor);
	if(BaseAI)
	{
		FDamageEvent DamageEvent;
		APlayerKatanaCharacter* OwnerCharacter = Cast<APlayerKatanaCharacter>(GetOwner());
		if(OwnerCharacter)
		{
			//添加打击感
			URougeDemoAnimInstance* AnimInstance = OwnerCharacter->GetRougeDemoAnimationInstance();
			if(AnimInstance)
			{
				//获取当前播放的蒙太奇
				UAnimMontage* CurrentActiveMontage = AnimInstance->GetCurrentActiveMontage();
				if(CurrentActiveMontage)
				{
					CurrentActiveMontage->RateScale = 1.f;
				}
			}
		}
	}*/
}

void AMeleeWeapon::ActivateWeaponAttack()
{
	AttackBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMeleeWeapon::DeactivateWeaponAttack()
{
	AttackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMeleeWeapon::PlayTrailFX()
{
	if(TrailFX)
	{
		FVector TrailLocation1 = GetWeaponMesh()->GetSocketLocation(FName("Trail_Part_1"));
		UGameplayStatics::SpawnEmitterAttached(TrailFX,GetWeaponMesh(),FName("None"),TrailLocation1,FRotator::ZeroRotator,FVector(1),EAttachLocation::KeepWorldPosition,true,EPSCPoolMethod::None,true);

		FVector TrailLocation2 = GetWeaponMesh()->GetSocketLocation(FName("Trail_Part_2"));
		UGameplayStatics::SpawnEmitterAttached(TrailFX,GetWeaponMesh(),FName("None"),TrailLocation2,FRotator::ZeroRotator,FVector(1),EAttachLocation::KeepWorldPosition,true,EPSCPoolMethod::None,true);

		TrailComp->BeginTrails(
			FName("Trail_Part_1"),
			FName("Trail_Part_2"),
			ETrailWidthMode_FromCentre,
			1.f);
	}
}

void AMeleeWeapon::EndTrailFX()
{
	TrailComp->EndTrails();
}


void AMeleeWeapon::BeginWeaponAttack(float NewAttackDelayTime, int NewAttackDelayCount)
{
	GetWeaponMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttackDelayTime = NewAttackDelayTime;
	AttackDelayCount = NewAttackDelayCount;
}

void AMeleeWeapon::EndWeaponAttack()
{
	GetWeaponMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
