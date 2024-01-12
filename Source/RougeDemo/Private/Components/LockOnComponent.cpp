// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LockOnComponent.h"

#include "AI/BaseAI.h"
#include "Character/RougeCharacter.h"
#include "Engine/Canvas.h"
#include "HUD/LockOnWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
ULockOnComponent::ULockOnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	FStringAssetReference MarketWidgetPath(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/RougeDemo/SRC/Blueprints/UI/Target/WBP_Mark.WBP_Mark'"));
	UWidget* MarketWidgetObj = Cast<UWidget>(StaticLoadObject(UWidget::StaticClass(), nullptr, *MarketWidgetPath.ToString()));
	if(MarketWidgetObj)
	{
		MarketWidgetClass = MarketWidgetObj->GetClass();
	}
	
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
	PlayerRougeDemoCharacter = Cast<ARougeCharacter>(GetOwner());

	//自动锁敌的一个方法，无法把Widget移动到Enemy上
	/*if(PlayerRougeDemoCharacter && MarketWidgetClass)
	{
		//创建锁敌UI
		MarketWidget = Cast<ULockOnWidget>(CreateWidget(GetWorld(),MarketWidgetClass));
		MarketWidget->AddToViewport();
	}*/
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
				//清除已经锁敌Enemy的Market
				ClearMarket(Enemy,false);
				
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
				ClearMarket(TempFrontTarget,true);
				
				//MarketWidget->ToggleMarket(LockOnTarget);
			}else if(TempBackTarget)
			{
				LockOnTarget = TempBackTarget;
				ClearMarket(TempFrontTarget,true);
				//MarketWidget->ToggleMarket(LockOnTarget);
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
		//MarketWidget->ToggleMarket(LockOnTarget);
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
		TArray<AActor*> IgnoreArr;
		IgnoreArr.Add(PlayerRougeDemoCharacter);
		CollisionParams.AddIgnoredActors(IgnoreArr); // 可根据需要添加需要忽略的其他物体
		
		//FQuat::Identity 单位四元数
		GetWorld()->SweepMultiByChannel(OutHits, Start, End, FQuat::Identity, ECC_Pawn,
			FCollisionShape::MakeSphere(RadarRange), CollisionParams);

		DrawDebugSphere(GetWorld(),Start,RadarRange,1,FColor::Green);
	}else
	{
		UE_LOG(LogTemp,Warning,TEXT("Player is nullptr,please check again"));
	}
	return OutHits;
}


AActor* ULockOnComponent::IdentifyTeam(FHitResult Hit)
{
	AActor* HitActor = Hit.GetActor();
	//比较Actor Tags，如果是同一队伍，则不进行锁敌 Floor会报错
	if(PlayerRougeDemoCharacter && HitActor && HitActor->Tags.Num() > 0 && PlayerRougeDemoCharacter->Tags.Num() > 0 && PlayerRougeDemoCharacter->Tags[0] != HitActor->Tags[0])
	{
		const FVector Start = PlayerRougeDemoCharacter->GetActorLocation();
		const FVector End = HitActor->GetActorLocation();

		//射线检测，是否可以看见，若不在视野范围内，则取消锁敌
		TArray<AActor*> InIgnoreActors;
		InIgnoreActors.Add(PlayerRougeDemoCharacter);
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActors(InIgnoreActors);
		FHitResult HitResult;
		
		//DrawDebugLine(GetWorld(),Start,End,FColor::Red,true);
		if(!GetWorld()->LineTraceSingleByChannel(HitResult,Start,End,ECC_Visibility,CollisionQueryParams))
		{
			return HitActor;
		}
	}
	
	return nullptr;
}

void ULockOnComponent::SetControlRotationTowardTarget()
{
	if(LockOnTarget)
	{
		const FVector Start = PlayerRougeDemoCharacter->GetActorLocation();
		const FVector Target = LockOnTarget->GetActorLocation();

		const FRotator PlayerLookToTargetRotation = UKismetMathLibrary::FindLookAtRotation(
			Start,
			Target
		);

		const FRotator CurrentRotation = PlayerRougeDemoCharacter->GetInstigator()->GetControlRotation();
		FRotator InterpRotation = UKismetMathLibrary::RInterpTo(
			CurrentRotation,
			PlayerLookToTargetRotation,
			UGameplayStatics::GetWorldDeltaSeconds(GetWorld()),
			RotationThickness
		);

		const FRotator ActorRotation(0.f,InterpRotation.Yaw,0.f);
		PlayerRougeDemoCharacter->SetActorRotation(ActorRotation);
	}
}

void ULockOnComponent::ActivateLockOnScreenPositionBased()
{
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

	if(Enemies.IsValidIndex(0))
	{
		FVector2D ViewPortSize;
		if(GEngine)
		{
			GEngine->GameViewport->GetViewportSize(ViewPortSize);
		}
		ScreenCenterPosition = ViewPortSize / 2;

		float LocalTargetScreenDistance = 10000.f;

		for(auto Enemy : Enemies)
		{
			if(Enemy->WasRecentlyRendered(0.2f))
			{
				const FVector EnemyLocation = Enemy->GetActorLocation();
				const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
				if(PlayerController)
				{
					FVector2D EnemyScreenPosition;
					//将3D坐标转换为2D坐标
					UGameplayStatics::ProjectWorldToScreen(PlayerController,EnemyLocation,EnemyScreenPosition,false);
					if((ScreenCenterPosition - EnemyScreenPosition).Size() < LocalTargetScreenDistance)
					{
						LocalTargetScreenDistance = (ScreenCenterPosition - EnemyScreenPosition).Size();
						TempScreenTarget = Enemy;
					}
				}

				//Clear ALl Market
			}
		}

		//激活UI并锁定
		if(TempScreenTarget)
		{
			LockOnTarget = TempScreenTarget;
			MarketWidget->ToggleMarket(LockOnTarget);
		}else
		{
			LockOnTarget = nullptr;
			MarketWidget->ToggleMarket(LockOnTarget);
		}
	}else
	{
		LockOnTarget = nullptr;
		//取消UI
		MarketWidget->ToggleMarket(LockOnTarget);
	}
}

void ULockOnComponent::ClearMarket(AActor* AIActor, bool bLockOn)
{
	ABaseAI* BaseAI = Cast<ABaseAI>(AIActor);
	if(BaseAI)
	{
		BaseAI->ToggleMarket(bLockOn);
	}
}

void ULockOnComponent::ExistTarget()
{
	if(PlayerRougeDemoCharacter && LockOnTarget)
	{
		const float Distance = PlayerRougeDemoCharacter->GetDistanceTo(LockOnTarget);
		if(Distance > RadarRange)
		{
			DeactivateLockOn();
		}
	}else
	{
		DeactivateLockOn();
	}
}

// Called every frame
void ULockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	SetControlRotationTowardTarget();
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
		bIsLockOn = true;
		SelectTarget();

		//将该组件的tick函数设置为启用
		SetComponentTickEnabled(true);

		//检测目标,开启TimerHandle
		GetWorld()->GetTimerManager().SetTimer(
			ExistTargetTimer,
			this,
			&ULockOnComponent::ExistTarget,
			0.5f,
			true
		);
		//设置为二维移动
		PlayerRougeDemoCharacter->bUseControllerRotationYaw = true;
	}
}

void ULockOnComponent::DeactivateLockOn()
{
	if(bIsAutoTargeting)
	{
		bIsLockOn = false;
		
		LockOnTarget = nullptr;
		//MarketWidget->ToggleMarket(LockOnTarget);

		
	}else
	{
		bIsLockOn = false;
		ClearMarket(LockOnTarget,false);
		LockOnTarget = nullptr;
		
		//MarketWidget->ToggleMarket(LockOnTarget);
		//将该组件的tick函数设置为启用
		SetComponentTickEnabled(false);

		//关闭TimerHandle
		GetWorld()->GetTimerManager().ClearTimer(ExistTargetTimer);
		//设置为二维移动
		PlayerRougeDemoCharacter->bUseControllerRotationYaw = false;
	}
}

