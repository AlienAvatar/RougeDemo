// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LockOnComponent.h"

#include "Blueprint/UserWidget.h"
#include "Character/RougeDemoCharacter.h"
#include "HUD/LockOnWidget.h"

// Sets default values for this component's properties
ULockOnComponent::ULockOnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULockOnComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Initialize();
}

void ULockOnComponent::Initialize()
{
	//只有玩家才能锁定
	PlayerRougeDemoCharacter = Cast<ARougeDemoCharacter>(GetOwner());
	if(PlayerRougeDemoCharacter && MarketWidgetClass)
	{
		//创建锁敌UI
		MarketWidget = Cast<ULockOnWidget>(CreateWidget(GetWorld(),MarketWidgetClass));
		MarketWidget->AddToViewport();
	}
}

void ULockOnComponent::SelectTarget()
{
	//查找范围的敌人
	TArray<FHitResult> OutHits = SearchTargets();
	for(FHitResult Hit: OutHits)
	{
		//对范围的友军不进行锁敌
		AActor* HitActor = IdentifyTeam(Hit);
		if(HitActor)
		{
			Enemies.AddUnique(HitActor);
		}
	}

	//范围内是否有敌人
	if(Enemies.IsValidIndex(0))
	{
		//以前方有敌人为优先级
		if(bHasFrontPriority)
		{
			FrontTargetDistance = RadarRange;
			BackTargetDistance = RadarRange;
			for(AActor* Enemy : Enemies)
			{
				//将Player和Enemy做点乘，如果大于0，证明敌人在Player前方，反之在后方
				if(PlayerRougeDemoCharacter->GetHorizontalDotProductTo(Enemy) > 0.f)
				{
					if(PlayerRougeDemoCharacter->GetDistanceTo(Enemy) < FrontTargetDistance)
					{
						//取得Player和Enemy的前方距离
						FrontTargetDistance = PlayerRougeDemoCharacter->GetDistanceTo(Enemy);
						TempFrontTarget = Enemy;
					}
				}else
				{
					if(PlayerRougeDemoCharacter->GetDistanceTo(Enemy) < BackTargetDistance)
					{
						//取得Player和Enemy的后方距离
						BackTargetDistance = PlayerRougeDemoCharacter->GetDistanceTo(Enemy);
						TempBackTarget = Enemy;
					}
				}
			}

			//激活UI并锁定
			if(TempFrontTarget)
			{
				LockOnTarget = TempFrontTarget;
				MarketWidget->ToggleMarket(LockOnTarget);
			}else if(TempBackTarget)
			{
				LockOnTarget = TempBackTarget;
				MarketWidget->ToggleMarket(LockOnTarget);
			}else
			{
				UE_LOG(LogTemp,Warning, TEXT("No target in current range or can't lock on target."));
			}
		}else
		{
			
		}
	}else
	{
		LockOnTarget = nullptr;
		//取消UI
		MarketWidget->ToggleMarket(LockOnTarget);
	}
}

TArray<FHitResult> ULockOnComponent::SearchTargets()
{
	TArray<FHitResult> OutHits;
	//根据玩家位置所在中心范围查找 35
	if(PlayerRougeDemoCharacter)
	{
		const FVector Start = PlayerRougeDemoCharacter->GetActorLocation();
		const FVector End = PlayerRougeDemoCharacter->GetActorLocation();

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(PlayerRougeDemoCharacter); // 可根据需要添加需要忽略的其他物体

		//FQuat::Identity 单位四元数
		GetWorld()->SweepMultiByChannel(OutHits, Start, End, FQuat::Identity, ECC_Visibility,
		   FCollisionShape::MakeSphere(RadarRange), CollisionParams);
	}else
	{
		UE_LOG(LogTemp,Warning,TEXT("Player is nullptr,please check again"));
	}
	return OutHits;
}


AActor* ULockOnComponent::IdentifyTeam(FHitResult Hit)
{
	AActor* HitActor = Hit.GetActor();
	//比较Actor Tags，如果是同一队伍，则不进行锁敌
	if(PlayerRougeDemoCharacter->Tags[0] != HitActor->Tags[0])
	{
		const FVector Start = PlayerRougeDemoCharacter->GetActorLocation();
		const FVector End = HitActor->GetActorLocation();

		//射线检测，是否可以看见，若不在视野范围内，则取消锁敌
		TArray<AActor*> InIgnoreActors;
		InIgnoreActors.Add(PlayerRougeDemoCharacter);
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActors(InIgnoreActors);
		FHitResult HitResult;
		if(!GetWorld()->LineTraceSingleByChannel(HitResult,Start,End,ECC_Visibility,CollisionQueryParams))
		{
			return HitActor;
		}
	}
	
	return nullptr;
}

// Called every frame
void ULockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULockOnComponent::ToggleLockOn()
{
	bIsLockOn ? DeactivateLockOn() : ActivateLockOn();
}

void ULockOnComponent::ActivateLockOn()
{
	if(bIsAutoTargeting)
	{
		
	}else
	{
		SelectTarget();
		if(LockOnTarget)
		{
			bIsLockOn = true;
		}
	}
}

void ULockOnComponent::DeactivateLockOn()
{
	if(bIsAutoTargeting)
	{
		bIsLockOn = false;
		LockOnTarget = nullptr;
		MarketWidget->ToggleMarket(LockOnTarget);
	}else
	{
		bIsLockOn = false;
		LockOnTarget = nullptr;
		MarketWidget->ToggleMarket(LockOnTarget);
	}
}

