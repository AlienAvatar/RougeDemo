// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BaseAI.h"

#include "AI/AIEnemyController.h"
#include "AI/BaseAIAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/RougeDemoCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetComponent.h"
#include "Core/RougeDemoGameMode.h"
#include "Core/RougeDemoPlayerController.h"
#include "Enum/EMovementDirection.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/EnemyHealthBarWidget.h"
#include "HUD/EnemyToughBarWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABaseAI::ABaseAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TargetWidget =  CreateDefaultSubobject<UWidgetComponent>(TEXT("TargetWidget"));
	TargetWidget->SetupAttachment(GetMesh());
	HealthWidget =  CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidget"));
	HealthWidget->SetupAttachment(GetMesh());
	ToughWidget =  CreateDefaultSubobject<UWidgetComponent>(TEXT("ToughWidget"));
	ToughWidget->SetupAttachment(GetMesh());
	FinisherWidget =  CreateDefaultSubobject<UWidgetComponent>(TEXT("FinisherWidget"));
	FinisherWidget->SetupAttachment(GetMesh(),FName("FinisherSocket"));
	FinisherWidget->SetVisibility(false);
	
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Tags.Add(FName("Enemy"));

	DissolveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DissolveTimelineComponent"));

	LeftAttackSphere = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftAttackSphere"));
	LeftAttackSphere->SetupAttachment(GetMesh(),FName("LeftAttackSocket"));
	LeftAttackSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftAttackSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	LeftAttackSphere->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	RightAttackSphere = CreateDefaultSubobject<UBoxComponent>(TEXT("RightAttackSphere"));
	RightAttackSphere->SetupAttachment(GetMesh(),FName("RightAttackSocket"));
	RightAttackSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightAttackSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	RightAttackSphere->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
}



// Called when the game starts or when spawned
void ABaseAI::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this,&ABaseAI::ReceiveDamage);

	BaseAIAnimInstance = Cast<UBaseAIAnimInstance>(GetMesh()->GetAnimInstance());

	//设置Health Bar
	SetHUDHealth();
	//设置Tough Bar
	SetHUDTough();
	
	//绑定碰撞
	LeftAttackSphere->OnComponentBeginOverlap.AddDynamic(this,&ABaseAI::OnLeftAttackBeginOverHandle);
	RightAttackSphere->OnComponentBeginOverlap.AddDynamic(this,&ABaseAI::OnRightAttackBeginOverHandle);

	AIController = Cast<AAIEnemyController>(GetController());
}

// Called every frame
void ABaseAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCharacterInfo(DeltaTime);
	UpdateGroundedRotation(DeltaTime);
}

// Called to bind functionality to input
void ABaseAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseAI::ToggleMarket(bool bLockOn)
{
	if(TargetWidget)
	{
		TargetWidget->SetVisibility(bLockOn);
	}
}

void ABaseAI::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatorController, AActor* DamageCauser)
{
	AttributeInfo.Health = FMath::Clamp(AttributeInfo.Health - Damage, 0.f, AttributeInfo.MaxHealth);
	AttributeInfo.ToughnessValue = FMath::Clamp(AttributeInfo.ToughnessValue + 50.f, 0.f, AttributeInfo.MaxToughnessValue);
	//UE_LOG(LogTemp,Warning,TEXT("AttributeInfo.ToughnessValue[%f]"),AttributeInfo.ToughnessValue);
	
	UpdateHUDHealth();
	UpdateHUDTough();

	//处于Stun状态，可以被角色处决
	if(AttributeInfo.Health > 0.f && AttributeInfo.ToughnessValue >= 100.f)
	{
		GetWorld()->GetTimerManager().SetTimer(
			ToughRecoverTimer,
			this,
			&ABaseAI::ToughRecoverTimerCallback,
			0.75f,
			true,
			1.f
		);
		State = EState::ES_Stun;
		GetCharacterMovement()->DisableMovement();

		FinisherWidget->SetVisibility(true);
	}
	
	if(AttributeInfo.Health == 0.f)
	{
		ARougeDemoGameMode* RougeDemoGameMode = GetWorld()->GetAuthGameMode<ARougeDemoGameMode>();
		if(RougeDemoGameMode)
		{
			EnemyController = EnemyController == nullptr ? Cast<AAIEnemyController>(Controller) : EnemyController;
			ARougeDemoPlayerController* AttackController = Cast<ARougeDemoPlayerController>(InstigatorController);
			
			RougeDemoGameMode->PlayEliminated(this,EnemyController,AttackController);
		}

		Dead();
	}else
	{
		PlayHitReactMontage();
		ReceDamageDirection(DamagedActor,DamageCauser);
	}
}

void ABaseAI::UpdateHUDHealth()
{
	SetHUDHealth();
}

void ABaseAI::UpdateHUDTough()
{
	SetHUDTough();
}

void ABaseAI::Elim()
{
	if(DissolveMaterialInstance)
	{
		//UMaterialInterface* DissolveMaterialInterface = GetMesh()->GetMaterial(0);
		DynamicDissolveMaterialInstance = UMaterialInstanceDynamic::Create(DissolveMaterialInstance,this);
		GetMesh()->SetMaterial(0,DynamicDissolveMaterialInstance);
		DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Dissolve"),0.55f);
		DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Glow"),35.f);
	}

	StartDissolve();
}

void ABaseAI::DestroyCallBack()
{
	Elim();
	//Destroy();

	GetWorld()->GetTimerManager().ClearTimer(DestroyTimerHandle);
}

void ABaseAI::SetHUDHealth()
{
	UEnemyHealthBarWidget* HealthBarWidget = Cast<UEnemyHealthBarWidget>(HealthWidget->GetUserWidgetObject());

	bool bHUDValid = HealthBarWidget &&
		HealthBarWidget->HealthBar;
	
	if(bHUDValid)
	{
		float HealthPercent = AttributeInfo.Health / AttributeInfo.MaxHealth;
		HealthBarWidget->HealthBar->SetPercent(HealthPercent);
	}
}

void ABaseAI::SetHUDTough()
{
	UEnemyToughBarWidget* ToughBarWidget = Cast<UEnemyToughBarWidget>(ToughWidget->GetUserWidgetObject());

	bool bHUDValid = ToughBarWidget &&
		ToughBarWidget->ToughBar;
	
	if(bHUDValid)
	{
		float ToughPercent = AttributeInfo.ToughnessValue / AttributeInfo.MaxToughnessValue;
		ToughBarWidget->ToughBar->SetPercent(ToughPercent);
	}
}

bool ABaseAI::CanUseAnyAbility()
{
	return IsAlive() && !UGameplayStatics::IsGamePaused(GetWorld());
}

float ABaseAI::PlayAttackMeleeMontage()
{
	if(BaseAIAnimInstance && AttackMontageRoot)
	{
		const float FinishAttackDelay = BaseAIAnimInstance->Montage_Play(AttackMontageRoot,1.f);
		BaseAIAnimInstance->Montage_JumpToSection(FName("ForwardAttack1"));
		return FinishAttackDelay;
	}
	return 0.f;
}

void ABaseAI::OnLeftAttackBeginOverHandle(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	
}

void ABaseAI::OnRightAttackBeginOverHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARougeDemoCharacter* OwnerCharacter = Cast<ARougeDemoCharacter>(OtherActor);
	if(OwnerCharacter)
	{
		AController* OwnerController = OwnerCharacter->Controller;
		if(OwnerController)
		{
			UGameplayStatics::ApplyDamage(OwnerCharacter,DamageCount,OwnerController,this,UDamageType::StaticClass());
		}
	}
}

void ABaseAI::ToughRecoverTimerCallback()
{
	AttributeInfo.ToughnessValue = FMath::Clamp(AttributeInfo.ToughnessValue - ToughRecoverAmount, 0.f, AttributeInfo.MaxToughnessValue);
	if(IsAlive() && AttributeInfo.ToughnessValue == 0.f)
	{
		State = EState::ES_Idle;
		GetWorld()->GetTimerManager().ClearTimer(ToughRecoverTimer);
		FinisherWidget->SetVisibility(false);
		GetCharacterMovement()->SetMovementMode(MOVE_MAX);
	}

	UpdateHUDTough();
}

EMovementDirection ABaseAI::ReceDamageDirection(AActor* DamagedActor, AActor* CauseActor)
{
	const FVector DamageVector = DamagedActor->GetActorLocation();
	const FVector CauseVector = CauseActor->GetActorLocation();
	
	FRotator Damage2CauseRotator = UKismetMathLibrary::FindLookAtRotation(
		DamageVector,
		CauseVector
	);
	float DirectionValue = UKismetMathLibrary::NormalizedDeltaRotator(
		Damage2CauseRotator,
		DamagedActor->GetActorRotation()
	).Yaw;

	UE_LOG(LogTemp,Warning,TEXT("DirectionValue[%f]"),DirectionValue);
	EMovementDirection Direction = EMovementDirection::EMD_Forward;

	
	return Direction;
}

void ABaseAI::SmoothCharacterRotation(FRotator Target, float TargetInterpSpeed, float ActorInterpSpeed)
{
	TargetRotation = UKismetMathLibrary::RInterpTo_Constant(
		TargetRotation,
		Target,
		UGameplayStatics::GetWorldDeltaSeconds(GetWorld()),
		TargetInterpSpeed);

	FRotator ActorRotation = UKismetMathLibrary::RInterpTo(
		GetActorRotation(),
		TargetRotation,
		UGameplayStatics::GetWorldDeltaSeconds(GetWorld()),
		ActorInterpSpeed);

	SetActorRotation(ActorRotation);
}

void ABaseAI::UpdateCharacterInfo(float DeltaTime)
{
	Velocity = GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();
}

void ABaseAI::UpdateDissolveMaterial(float DissolveValue)
{
	if(DynamicDissolveMaterialInstance)
	{
		DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Dissolve"),DissolveValue);
	}
}

void ABaseAI::StartDissolve()
{
	DissolveTrack.BindDynamic(this,&ABaseAI::UpdateDissolveMaterial);
	if(DissolveCurve && DissolveTimeline)
	{
		DissolveTimeline->AddInterpFloat(DissolveCurve,DissolveTrack);
		DissolveTimeline->Play();
	}
}

void ABaseAI::PlayElimMontage()
{
	if(BaseAIAnimInstance && F_ElimMontageRoot)
	{
		FName MontageSection = TEXT("Forward");
		BaseAIAnimInstance->Montage_Play(F_ElimMontageRoot);
		BaseAIAnimInstance->Montage_JumpToSection(MontageSection,F_ElimMontageRoot);
	}
}

void ABaseAI::UpdateGroundedRotation(float DeltaTime)
{
	const UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
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

			UE_LOG(LogTemp,Warning,TEXT("DireValue[%f]"),DireValue);
			//true 玩家在前面
			if(DireValue > 0.f)
			{
				//如果Player在玩家前面执行AO
				//To Do 计算YawOffset
				
			}else
			{
				FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(
				this->GetActorLocation(),
				TargetActor->GetActorLocation()
				);
				//如果Player在玩家后面执行旋转
				const FRotator YawRotation(0.f,Rotation.Yaw,0.f);

				const float RotationRate = 1.f;
				SmoothCharacterRotation(YawRotation,800.f,RotationRate);
			}
		}
	}
}

float ABaseAI::OnTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation,
                                 UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, AActor* DamageCauser,
                                 AController* InstigatedByController, AActor* DamageCauserActor)
{
	UE_LOG(LogTemp,Warning,TEXT("OnTakePointDamage Damage[%f]"),Damage);
	return 0.f;
}

float ABaseAI::InternalTakePointDamage(float Damage, FPointDamageEvent const& PointDamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	AttributeInfo.Health = AttributeInfo.Health - Damage;
	if(AttributeInfo.Health <= 0.f)
	{
		Dead();
	}
	
	return Super::InternalTakePointDamage(Damage, PointDamageEvent, EventInstigator, DamageCauser);
}

void ABaseAI::Dead()
{
	//解除锁定
	ToggleMarket(false);

	State = EState::ES_Dead;
	
	GetMesh()->SetCollisionObjectType(ECC_PhysicsBody);
	//模拟物理
	GetMesh()->SetSimulatePhysics(true);

	//2s后调用
	GetWorld()->GetTimerManager().SetTimer(
		DestroyTimerHandle,
		this,
		&ABaseAI::DestroyCallBack,
		2.f,
		false
	);

	//隐藏Health Widget
	HealthWidget->SetVisibility(false);
	
	Elim();
}

bool ABaseAI::DoMeleeAttack(float& Delay)
{
	const ARougeDemoCharacter* Player = Cast<ARougeDemoCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	if(Player && Player->IsAlive())
	{
		if(CanUseAnyAbility())
		{
			const float DelaySeconds = PlayAttackMeleeMontage();
			Delay = DelaySeconds;
			return true;
		}
	}
	return false;
}

void ABaseAI::ActivateLeftAttack()
{
	LeftAttackSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ABaseAI::ActivateRightAttack()
{
	RightAttackSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ABaseAI::DeactivateLeftAttack()
{
	LeftAttackSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseAI::DeactivateRightAttack()
{
	RightAttackSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseAI::ActivateRangeAttack()
{
	
	PlayRangeAttackAnim();
}

void ABaseAI::PlayRangeAttackAnim()
{
	UBaseAIAnimInstance* AnimInstance = Cast<UBaseAIAnimInstance>(GetMesh()->GetAnimInstance());
	if(AnimInstance)
	{
		if(RangeAttackMontageRoot)
		{
			AnimInstance->Montage_Play(RangeAttackMontageRoot);
		}
	}
}

void ABaseAI::SpawnProjectile(AActor* TargetActor)
{
	//获取MuzzleSocket
	FVector MuzzleLocation = GetMesh()->GetSocketLocation("MuzzleSocket");
	FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
	FRotator MuzzleRotation = Direction.Rotation();

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Params.Instigator = this;

	AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, Params);
}

void ABaseAI::PlayHitReactMontage()
{
	UBaseAIAnimInstance* AnimInstance = Cast<UBaseAIAnimInstance>(GetMesh()->GetAnimInstance());
	if(AnimInstance)
	{
		if(F_HitMontage)
		{
			AnimInstance->Montage_Play(F_HitMontage);
		}
	}
}



