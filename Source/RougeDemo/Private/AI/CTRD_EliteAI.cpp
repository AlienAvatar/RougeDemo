// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CTRD_EliteAI.h"

#include "AI/AIEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/RougeDemoCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"

ACTRD_EliteAI::ACTRD_EliteAI()
{
	//RayAttackTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("RayAttackTimeLine"));

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/RougeDemo/SRC/Data/Curves/AI/Attack/RayAttackCurve"));
	check(Curve.Succeeded());
    
	RayAttackCurve = Curve.Object;

	RightLaser = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightLaserComp"));
	RightLaser->SetupAttachment(GetMesh(),FName("RightRaySocket"));
	RightLaser->SetRelativeScale3D(FVector(0.025f,0.025,2.f));
	RightLaser->SetRelativeRotation(FRotator(0.f,90.f,-90.f));
	RightLaser->SetCollisionResponseToAllChannels(ECR_Ignore);
	RightLaser->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightLaser->SetVisibility(false);
	
	LeftLaser = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftLaserComp"));
	LeftLaser->SetupAttachment(GetMesh(),FName("LeftRaySocket"));
	LeftLaser->SetRelativeScale3D(FVector(0.025f,0.025,2.f));
	LeftLaser->SetRelativeRotation(FRotator(0.f,90.f,-90.f));
	LeftLaser->SetCollisionResponseToAllChannels(ECR_Ignore);
	LeftLaser->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftLaser->SetVisibility(false);
	
	RightLaser->OnComponentBeginOverlap.AddDynamic(this,&ACTRD_EliteAI::ACTRD_EliteAI::OnBeginOverlap);
	LeftLaser->OnComponentBeginOverlap.AddDynamic(this,&ACTRD_EliteAI::ACTRD_EliteAI::OnBeginOverlap);
	RightLaser->OnComponentHit.AddDynamic(this,&ACTRD_EliteAI::ACTRD_EliteAI::OnHit);
	LeftLaser->OnComponentHit.AddDynamic(this,&ACTRD_EliteAI::ACTRD_EliteAI::OnHit);
}

void ACTRD_EliteAI::BeginPlay()
{
	Super::BeginPlay();
	
	FOnTimelineFloat OnTimelineCallback;
	FOnTimelineEventStatic OnTimelineFinishedCallback;

	if (RayAttackCurve)
	{
		RayAttackTimeLine = NewObject<UTimelineComponent>(this, FName("TimelineAnimation"));
		RayAttackTimeLine->CreationMethod = EComponentCreationMethod::UserConstructionScript; // Indicate it comes from a blueprint so it gets cleared when we rerun construction scripts
		this->BlueprintCreatedComponents.Add(RayAttackTimeLine); // Add to array so it gets saved
		RayAttackTimeLine->SetNetAddressable();	// This component has a stable name that can be referenced for replication

		RayAttackTimeLine->SetPropertySetObject(this); // Set which object the timeline should drive properties on
		RayAttackTimeLine->SetDirectionPropertyName(FName("TimelineDirection"));

		RayAttackTimeLine->SetLooping(false);
		RayAttackTimeLine->SetTimelineLength(CurveLenTime);
		RayAttackTimeLine->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);

		//RayAttackTimeLine->SetPlaybackPosition(0.0f, false);

		//Add the float curve to the timeline and connect it to your timelines's interpolation function
		OnTimelineCallback.BindUFunction(this, FName{ TEXT("UpdateRayAttack") });
		OnTimelineFinishedCallback.BindUFunction(this, FName{ TEXT("OnTimelineEndEvent") });
		
		RayAttackTimeLine->AddInterpFloat(RayAttackCurve, OnTimelineCallback);
		RayAttackTimeLine->SetTimelineFinishedFunc(OnTimelineFinishedCallback);

		RayAttackTimeLine->RegisterComponent();
	}

	BlackboardComponent = AIController->GetBlackboardComponent();
}

void ACTRD_EliteAI::Tick(float DeltaSeconds)
{
	UpdateCharacterInfo(DeltaSeconds);
	if(!bIsRayAttacking)
	{
		UpdateGroundedRotation(DeltaSeconds);
	}

	if (RayAttackTimeLine)
	{
		RayAttackTimeLine->TickComponent(DeltaSeconds, ELevelTick::LEVELTICK_TimeOnly, NULL);
	}
	CacheValues(DeltaSeconds);
}

void ACTRD_EliteAI::UpdateGroundedRotation(float DeltaTime)
{
	if(BlackboardComponent)
	{
		//获取TargetActor，一般为Player
		AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject("TargetActor"));
		if(TargetActor)
		{
			//计算玩家方位
			float DireValue = UKismetMathLibrary::Dot_VectorVector(
				TargetActor->GetActorLocation(),
				this->GetActorLocation()
			);
			
			//获取头的位置
			const FVector HeadLocation = this->GetMesh()->GetBoneLocation(FName("head"));
			const FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(
				HeadLocation,
				TargetActor->GetActorLocation()
			);

			//如果Player在玩家前面执行AO
			const FRotator DeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(LookRotation,GetActorRotation());
			const float LocalTargetYawOffset = FMath::Clamp(DeltaRotator.Yaw,-180.f,180.f);
			const float LocalTargetPitchOffset = FMath::Clamp(DeltaRotator.Pitch,-180.f,180.f);
			
			//发射完射线后，平滑处理Yaw，Pitch
			YawOffset = UKismetMathLibrary::FInterpTo(
			YawOffset,
			LocalTargetYawOffset,
			UGameplayStatics::GetWorldDeltaSeconds(GetWorld()),
			5.f);
		
			PitchOffset = UKismetMathLibrary::FInterpTo(
			PitchOffset,
			LocalTargetPitchOffset,
			UGameplayStatics::GetWorldDeltaSeconds(GetWorld()),
			5.f);

			//计算AimYawRate
			AimYawRate = FMath::Abs((GetBaseAimRotation().Yaw - PreviousAimYaw) / DeltaTime);

			//Test
			if(YawOffset <= -70.f || YawOffset >= 70.f)
			{
				const FRotator YawRotation(0.f,LookRotation.Yaw,0.f);

				const float RotationRate = 1.f;
				SmoothCharacterRotation(YawRotation,800.f,RotationRate);
			}

			//需要计算距离
			//问题：距离太近会出现抖动
			
			
			/*//true 玩家在前面
			if(DireValue > 0.f)
			{
				
			}else
			{
				UE_LOG(LogTemp,Warning,TEXT("DireValue[%f]"),DireValue);
				//如果Player在玩家后面执行旋转
				
			}*/
		}
	}
}

void ACTRD_EliteAI::UpdateRayAttack(float NewPitchValue)
{
	//限制Ray的角度
	PitchOffset = FMath::Clamp(PitchOffset += NewPitchValue,-30.f,10.f);

	const FVector LeftRaySocketLocation = GetMesh()->GetSocketLocation(FName("LeftRaySocket"));
	const FVector RightRaySocketLocation = GetMesh()->GetSocketLocation(FName("RightRaySocket"));

	const FRotator LeftRaySocketRotation = GetMesh()->GetSocketRotation(FName("LeftRaySocket"));
	const FRotator RightRaySocketRotation = GetMesh()->GetSocketRotation(FName("RightRaySocket"));
	const FVector LeftDirection(FRotationMatrix(LeftRaySocketRotation).GetUnitAxis(EAxis::X));
	const FVector RightDirection(FRotationMatrix(RightRaySocketRotation).GetUnitAxis(EAxis::X));
	
	const FVector LeftRaySocketLocationEnd = LeftRaySocketLocation + LeftDirection * 50000.f;
	const FVector RightRaySocketLocationEnd = RightRaySocketLocation + RightDirection * 50000.f;
	
	FHitResult LeftHit;
	GetWorld()->LineTraceSingleByChannel(
		LeftHit,
		LeftRaySocketLocation,
		LeftRaySocketLocationEnd,
		ECC_Pawn
	);
	FHitResult RightHit;
	GetWorld()->LineTraceSingleByChannel(
		RightHit,
		RightRaySocketLocation,
		RightRaySocketLocationEnd,
		ECC_Pawn
	);
	
	//发射粒子效果
	//PlayRayAttackFX(LeftRaySocketLocation,LeftRaySocketRotation,LeftHit);
	//PlayRayAttackFX(RightRaySocketLocation,RightRaySocketRotation,RightHit);
	
	//射线检测
	//DrawDebugLine(GetWorld(),LeftRaySocketLocation,LeftRaySocketLocationEnd,FColor::Red,true,10.f);
	//DrawDebugLine(GetWorld(),RightRaySocketLocation,RightRaySocketLocationEnd,FColor::Red,true,10.f);

	/*
	*/
	
}

void ACTRD_EliteAI::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		ARougeDemoCharacter* RougeDemoCharacter = Cast<ARougeDemoCharacter>(OtherActor);
		if(RougeDemoCharacter && RougeDemoCharacter->ActorHasTag(FName("Player")))
		{
			UGameplayStatics::ApplyDamage(RougeDemoCharacter,RayAttackDamageCount,GetInstigatorController(),this,UDamageType::StaticClass());
		}
	}
}

void ACTRD_EliteAI::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp,Warning,TEXT("OnHit"));
	UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(OtherComp);
	if(MeshComponent)
	{
		FVector HitLocation = Hit.Location;
		if(RayAttackHitFX)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),RayAttackHitFX,HitLocation);
		}
	}
}

void ACTRD_EliteAI::OnTimelineEndEvent(float Output)
{
	bIsRayAttacking = false;

	LeftLaser->SetVisibility(false);
	RightLaser->SetVisibility(false);
	LeftLaser->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightLaser->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACTRD_EliteAI::CacheValues(float DeltaTime)
{
	PreviousAimYaw = GetBaseAimRotation().Yaw;
	/*if(BlackboardComponent)
	{
		//获取TargetActor，一般为Player
		AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject("TargetActor"));
		if(TargetActor)
		{
			const FRotator LocalLookRotation = UKismetMathLibrary::FindLookAtRotation(
				GetActorLocation(),
				TargetActor->GetActorLocation()
			);
			PreviousAimYaw = LocalLookRotation.Yaw;
		}
	}*/
	
}

void ACTRD_EliteAI::PlayRayAttackFX(FVector Location, FRotator Rotation, FHitResult HitResult)
{
	if(RayAttackFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),RayAttackFX,Location,Rotation);	
	}

	if(HitResult.bBlockingHit)
	{
		const FVector HitLocation = HitResult.Location;
		if(RayAttackHitFX)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),RayAttackHitFX,HitLocation);
		}
	}
}

void ACTRD_EliteAI::CheckRayTrack(FHitResult HitResult)
{
	if(HitResult.bBlockingHit)
	{
		ARougeDemoCharacter* RougeDemoCharacter = Cast<ARougeDemoCharacter>(HitResult.GetActor());
		if(RougeDemoCharacter && RougeDemoCharacter->ActorHasTag(FName("Player")))
		{
			UGameplayStatics::ApplyDamage(RougeDemoCharacter,RayAttackDamageCount,GetInstigatorController(),this,UDamageType::StaticClass());
		}
	}
}

void ACTRD_EliteAI::MoveRayAttackHandleCallBack()
{
	if(RayAttackTimeLine)
	{
		RayAttackTimeLine->PlayFromStart();
	}
}

void ACTRD_EliteAI::RayAttack()
{
	bIsRayAttacking = true;
	
	LeftLaser->SetVisibility(true);
	RightLaser->SetVisibility(true);
	LeftLaser->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RightLaser->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	GetWorld()->GetTimerManager().SetTimer(
		MoveRayAttackTimerHandle,
		this,
		&ACTRD_EliteAI::MoveRayAttackHandleCallBack,
		0.2f,
		false,
		0.5f
	);
}
