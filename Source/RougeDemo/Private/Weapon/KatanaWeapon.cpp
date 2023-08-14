// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/KatanaWeapon.h"

#include "AI/BaseAI.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AKatanaWeapon::AKatanaWeapon()
{
	
}

void AKatanaWeapon::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(
		CheckDamageTimerHandle,
		this,
		&AKatanaWeapon::CheckDamage,
		0.02f,
		true
	);
}

void AKatanaWeapon::CheckDamage()
{
	//计算刀的速度
	FVector LocalLastLocationKatana = LastLocationKatana - GetWeaponMesh()->GetComponentLocation();
	float KatanaSpeed = LocalLastLocationKatana.Length();
	UE_LOG(LogTemp,Warning,TEXT("KatanaSpeed[%f]"),KatanaSpeed);
	if(KatanaSpeed < 35.f)
	{
		return;
	}
	TArray<USceneComponent*> ChildCompArr;
	WeaponMesh->GetChildrenComponents(false,ChildCompArr);

	for(int i = 0; i < ChildCompArr.Num(); ++i)
	{
		Start = ChildCompArr[i]->GetComponentLocation();
		FVector End = FVector::ZeroVector;
		ChildCompIndex = i;
		if(LastLocationArr.IsValidIndex(i))
		{
			End = LastLocationArr[i];
		};

		TArray<FHitResult> OutHitResult;
		TArray<AActor*> ActorsToIgnore;
		
		UKismetSystemLibrary::LineTraceMulti(GetWorld(),Start,End,ETraceTypeQuery::TraceTypeQuery2,false,ActorsToIgnore,EDrawDebugTrace::ForDuration,OutHitResult,true);
		//GetWorld()->LineTraceSingleByChannel(OutHitResult,Start,End,ECC_Visibility);

		for(auto OutHit : OutHitResult)
		{
			if(OutHit.bBlockingHit)
			{
				FVector HitFromDirection = End - Start;
				//归一化后是方向，不归一化是速度
				UKismetMathLibrary::Vector_Normalize(HitFromDirection);

				ABaseAI* Enemy = Cast<ABaseAI>(OutHit.GetActor());
				if(Enemy)
				{
					UGameplayStatics::ApplyPointDamage(
					Enemy,
					10.f,
					HitFromDirection,
					OutHit,
					GetInstigatorController(),
					GetOwner(),
					UDamageType::StaticClass()
				);
				}
				
			}
		}

		SetLastLocationArrayItem();
	}
}
