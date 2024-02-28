// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RougeCharacter.h"

#include "AbilitySystemComponent.h"
#include "BlueprintGameplayTagLibrary.h"
#include "AI/BaseAI.h"
#include "AI/BaseAIAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Character/RougeAnimInstance.h"
#include "Components/MagicComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CombatComponent.h"
#include "Components/LockOnComponent.h"
#include "Components/SphereComponent.h"
#include "..\..\Public\Core\RougeGameMode.h"
#include "..\..\Public\Core\RougePlayerController.h"
#include "AbilitySystem/RougeAbilitySystemComponent.h"
#include "Character/RougePawnExtensionComponent.h"
#include "Curves/CurveVector.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Lib/ReceiveDamageLibrary.h"
#include "Struct/AttributeInfo.h"
#include "Weapon/Weapon.h"
#include "../RougeGameplayTags.h"
#include "Character/RougeHealthComponent.h"
#include "Components/RougeCharacterMovementComponent.h"


// Sets default values
ARougeCharacter::ARougeCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//设置Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 315.f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0.f,75.f,0.f);
	CameraBoom->SetRelativeLocation(FVector(0.f,0.f,110.f));
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);

	LockOnComp = CreateDefaultSubobject<ULockOnComponent>(TEXT("LockOnComp"));
	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComp"));

	MovementState = EMovementState::EMS_Grounded;

	GetMesh()->SetRelativeLocation(FVector(0.f,0.f,-88.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f,-90.f,0.f));

	UDataTable* MovementModelDataTableRef = LoadObject<UDataTable>(nullptr, TEXT("/Game/RougeDemo/SRC/Data/DataTables/DT_MovementModel.DT_MovementModel"));
	if(MovementModelDataTableRef)
	{
		MovementModelDataTable = MovementModelDataTableRef;
	}

	Tags.Add(FName("Player"));

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	PawnExtComponent = CreateDefaultSubobject<URougePawnExtensionComponent>(TEXT("PawnExtensionComponent"));
	PawnExtComponent->OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemInitialized));
	PawnExtComponent->OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemUninitialized));

	HealthComponent = CreateDefaultSubobject<URougeHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnDeathStarted.AddDynamic(this, &ThisClass::OnDeathStarted);
	HealthComponent->OnDeathFinished.AddDynamic(this, &ThisClass::OnDeathFinished);
}




// Called when the game starts or when spawned
void ARougeCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnBeginPlay();
	
}

void ARougeCharacter::OnBeginPlay()
{
	//确保动画更新在Character后
	GetMesh()->AddTickPrerequisiteActor(this);

	//设置动画实例
	RougeDemoAnimInstance = Cast<URougeAnimInstance>(GetMesh()->GetAnimInstance());
	//设置Controller
	RougeDemoPlayerController = Cast<ARougePlayerController>(Controller);
	//绑定受击函数
	OnTakeAnyDamage.AddDynamic(this,&ARougeCharacter::ReceiveDamage);

	//设置当前血量
	if(RougeDemoPlayerController)
	{
		RougeDemoPlayerController->SetHUDHealth(AttributeInfo.Health,AttributeInfo.MaxHealth);
	}
	
	//设置默认状态
	Gait = DesiredGait;
	RotationMode = DesiredRotationMode;
	
	//读取DataTable
	SetMovementModel();
	
	//设置默认初始值
	CurrentTargetRotation = GetActorRotation();
	LastVelocityRotation = GetActorRotation();
	LastMovementInputRotation = GetActorRotation();

	//翻滚TimerHandle
	GetWorld()->GetTimerManager().SetTimer(
		RollTimerHandle,
		this,
		&ARougeCharacter::RollTimerHandlerCallback,
		0.01f,
		true
	);

	GetWorld()->GetTimerManager().SetTimer(
		StartTimerHandle,
		this,
		&ARougeCharacter::StartTimerHandleCallback,
		0.2f,
		false,
		0
	);
}

void ARougeCharacter::OnAbilitySystemInitialized()
{
	URougeAbilitySystemComponent* RougeASC = GetRougeAbilitySystemComponent();
	check(RougeASC);

	//初始化HealthComponent
	HealthComponent->InitializeWithAbilitySystem(RougeASC);
	
	//初始化Gameplay Tags
	InitializeGameplayTags();
}

void ARougeCharacter::OnAbilitySystemUninitialized()
{
}

void ARougeCharacter::InitializeGameplayTags()
{
	if (URougeAbilitySystemComponent* RougeASC = GetRougeAbilitySystemComponent())
	{
		const FRougeGameplayTags& GameplayTags = FRougeGameplayTags::Get();

		//遍历移动模式Map
		for (const TPair<uint8, FGameplayTag>& TagMapping : GameplayTags.MovementModeTagMap)
		{
			if (TagMapping.Value.IsValid())
			{
				RougeASC->SetLooseGameplayTagCount(TagMapping.Value, 0);
			}
		}
	}
}

void ARougeCharacter::SetMovementModeTag(EMovementMode MovementMode, uint8 CustomMovementMode, bool bTagEnabled)
{
	if (URougeAbilitySystemComponent* RougeASC = GetRougeAbilitySystemComponent())
	{
		const FGameplayTag* MovementModeTag = nullptr;
		const FRougeGameplayTags& GameplayTags = FRougeGameplayTags::Get();

		if (MovementMode == MOVE_Custom)
		{
			MovementModeTag = GameplayTags.CustomMovementModeTagMap.Find(CustomMovementMode);
		}
		else
		{
			MovementModeTag = GameplayTags.MovementModeTagMap.Find(MovementMode);
		}

		if (MovementModeTag && MovementModeTag->IsValid())
		{
			RougeASC->SetLooseGameplayTagCount(*MovementModeTag, (bTagEnabled ? 1 : 0));
		}
	}

	URougeCharacterMovementComponent* RougeMoveComp = CastChecked<URougeCharacterMovementComponent>(GetCharacterMovement());
	SetMovementModeTag(RougeMoveComp->MovementMode, RougeMoveComp->CustomMovementMode, true);
}

void ARougeCharacter::PossessedBy(AController* NewController)
{
	//const FGenericTeamId OldTeamID = MyTeamID;

	Super::PossessedBy(NewController);

	PawnExtComponent->HandleControllerChanged();
}

void ARougeCharacter::UnPossessed()
{
	Super::UnPossessed();
}

void ARougeCharacter::OnDeathStarted(AActor* OwningActor)
{
	RagdollStart();
	//禁止输入，清除碰撞
	//DisableMovementAndCollision();
}

void ARougeCharacter::OnDeathFinished(AActor* OwningActor)
{
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::DestroyDueToDeath);
}

void ARougeCharacter::DisableMovementAndCollision()
{
	if (Controller)
	{
		Controller->SetIgnoreMoveInput(true);
	}

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
}

void ARougeCharacter::DestroyDueToDeath()
{
	RagdollEnd();
	K2_OnDeathFinished();

	UninitAndDestroy();
}

void ARougeCharacter::UninitAndDestroy()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		DetachFromControllerPendingDestroy();
		SetLifeSpan(0.1f);
	}

	// Uninitialize the ASC if we're still the avatar actor (otherwise another pawn already did it when they became the avatar actor)
	if (URougeAbilitySystemComponent* RougeASC = GetRougeAbilitySystemComponent())
	{
		if (RougeASC->GetAvatarActor() == this)
		{
			PawnExtComponent->UninitializeAbilitySystem();
		}
	}

	SetActorHiddenInGame(true);
}

void ARougeCharacter::StartTimerHandleCallback()
{
	GetWorld()->GetTimerManager().SetTimer(
		AbilityLoopTimerHandle,
		this,
		&ARougeCharacter::AbilityLoopTimerHandleCallback,
		5.f,
		true,
		-5.f
	);
	GetWorld()->GetTimerManager().ClearTimer(StartTimerHandle);
}

void ARougeCharacter::AbilityLoopTimerHandleCallback()
{
	//Init Ability
	const bool bIsLocallyControlled = IsLocallyControlled();
	if(bIsLocallyControlled)
	{
		const URougePawnData* PawnData = nullptr;
		if (URougePawnExtensionComponent* PawnExtComp = URougePawnExtensionComponent::FindPawnExtensionComponent(this))
		{
			const FRougeGameplayTags& StartMagicTags = FRougeGameplayTags::Get();
			if(URougeAbilitySystemComponent* RougeASC = PawnExtComp->GetRougeAbilitySystemComponent())
			{
				FGameplayTagContainer StartContainer = UBlueprintGameplayTagLibrary::MakeGameplayTagContainerFromTag(StartMagicTags.Ability_Type_Action_Lighting);
				bool bSuccess = RougeASC->TryActivateAbilitiesByTag(StartContainer,true);
				//Log Test
				//FString Success = bSuccess ? "true" : "false";
				//UE_LOG(LogTemp, Warning, TEXT("Success[%s]"), *Success);
			}
		}
	}
}

// Called every frame
void ARougeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//AimOffset(DeltaTime);
	
	/*SetEssentialValues(DeltaTime);
	CacheValues(DeltaTime);

	//在锁定状态下启用
	if(LockOnComp->GetIsLockOn())
	{
		AActor* TargetActor = LockOnComp->GetLockOnTarget();
		AimOffset2Target(DeltaTime,TargetActor);
		
		FRotator CameraRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),TargetActor->GetActorLocation());
		//CameraBoom->SetWorldRotation(CameraRotation);
	}
	
	switch (MovementState)
	{
	case EMovementState::EMS_Grounded:
		UpdateCharacterMovement(DeltaTime);
		UpdateGroundedRotation(DeltaTime);
		break;
	case EMovementState::EMS_InAir:
		//To Do Something In Air
		break;
	case EMovementState::EMS_RagDoll:
		RagdollUpdate(DeltaTime);
		break;
	default:
		UE_LOG(LogTemp,Error,TEXT("No MovementState setting, Please check it."));
	}*/
	
	//DebugMessage
	//UE_LOG(LogTemp,Warning,TEXT("LastRagdollVelocity[%f]"),LastRagdollVelocity.Size());
	//UE_LOG(LogTemp,Warning,TEXT("DeltaRotation[%f]"),DeltaDirectRotation.Yaw);
}

EMovementDirection ARougeCharacter::CalculateInputDirection()
{
	//前进输入
	if(MoveF == 1.0 && MoveR == 0.f)
	{
		return EMovementDirection::EMD_Forward;
	}else if(MoveF == -1.f && MoveR == 0.f)
	{
		return EMovementDirection::EMD_Backward;
	}else if(MoveF == 0.f && MoveR == 1.f)
	{
		return EMovementDirection::EMD_Right;
	}else if(MoveF == 0.f && MoveR == -1.f)
	{
		return EMovementDirection::EMD_Left;
	}else if(MoveF == 1.f && MoveR == 1.f)
	{
		return EMovementDirection::EMD_ForwardRight;
	}else if(MoveF == 1.f && MoveR == -1.f)
	{
		return EMovementDirection::EMD_ForwardLeft;
	}else if(MoveF == -1.f && MoveR == 1.f)
	{
		return EMovementDirection::EMD_BackwardRight;
	}else if(MoveF == -1.f && MoveR == -1.f)
	{
		return EMovementDirection::EMD_ForwardLeft;
	}else
	{
		return EMovementDirection::EMD_MAX;
	}
}

void ARougeCharacter::AdjustPassive(EPassiveAbilities Stat, float MultiplicationAmount)
{
	switch (Stat)
	{
		case EPassiveAbilities::EPA_Speed:
			{
				float MaxSpeed = GetCharacterMovement()->MaxWalkSpeed;
				MaxSpeed *= MultiplicationAmount;
				GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
			}
			break;
		case EPassiveAbilities::EPA_Health:
			float MaxHealth = AttributeInfo.MaxHealth;
			MaxHealth *= MultiplicationAmount;
			AttributeInfo.MaxHealth = MaxHealth;
			UpdateHealthHUD();
			break;
	}
}

void ARougeCharacter::RestoreHealth(float Health)
{
	AttributeInfo.Health += Health;
	UpdateHealthHUD();
}

void ARougeCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                    AController* InstigatorController, AActor* DamageCauser)
{
	//角色不可受到连续攻击
	if(!bHitting)
	{
		bHitting = true;
		AttributeInfo.Health = FMath::Clamp(AttributeInfo.Health - Damage, 0.f, AttributeInfo.MaxHealth);

		UpdateHealthHUD();
		//判断DamageCauser的受击方向
		const EMovementDirection LocalHitDirection = UReceiveDamageLibrary::CalculateDamageCauserDirection(DamagedActor,DamageCauser);
		UE_LOG(LogTemp, Warning, TEXT("DamageType[%s]"), *DamageType->GetName());
		//UE_LOG(LogTemp, Warning, TEXT("DamageType.DestructibleImpulse[%f]"), DamageType->);
		
		//判断伤害类型，目前判断的是一次攻击的削韧强度
		ABaseAI* BaseAI = Cast<ABaseAI>(DamagedActor);
		if(BaseAI)
		{
			UBaseAIAnimInstance* BaseAIAnimInstance = BaseAI->GetBaseAIAnimInstance();
			if(BaseAIAnimInstance)
			{
				UAnimMontage* CurrentActiveMontage = BaseAIAnimInstance->GetCurrentActiveMontage();
				if(CurrentActiveMontage)
				{
					
				}
			}
		}
		//To do 判断是Projectile的攻击
		
		//根据受击方向播放动画
		PlayHitReactMontage(LocalHitDirection);
	}
	
	if(AttributeInfo.Health == 0.f)
	{
		RagdollAction();
	}
}

void ARougeCharacter::UpdateHealthHUD()
{
	//更改UMG
	if(RougeDemoPlayerController)
	{
		RougeDemoPlayerController->SetHUDHealth(AttributeInfo.Health,AttributeInfo.MaxHealth);
	}
}

void ARougeCharacter::PlayHitReactMontage(EMovementDirection HitDirection)
{
	URougeAnimInstance* AnimInstance = Cast<URougeAnimInstance>(GetMesh()->GetAnimInstance());
	if(AnimInstance)
	{
		UAnimMontage* HitMontage = nullptr;
		switch(HitDirection)
		{
		case EMovementDirection::EMD_Forward:
			HitMontage = AttributeInfo.F_HitMontage;
			break;
		case EMovementDirection::EMD_Left:
			HitMontage = AttributeInfo.L_HitMontage;
			break;
		case EMovementDirection::EMD_Right:
			HitMontage = AttributeInfo.R_HitMontage;
			break;
		case EMovementDirection::EMD_Backward:
			HitMontage = AttributeInfo.B_HitMontage;
			break;
		default:
			UE_LOG(LogTemp,Error,TEXT("No Montage to found"));
			return;
		}
		
		if(HitMontage)
		{
			AnimInstance->Montage_Play(HitMontage);
		}
	}
}

void ARougeCharacter::RagdollUpdate(float DeltaTime)
{
	LastRagdollVelocity = GetMesh()->GetPhysicsLinearVelocity(FName("root"));

	//使用布娃娃的速度来缩放布娃娃骨骼关节
	const float OutVelocityValue = UKismetMathLibrary::MapRangeClamped(
		LastRagdollVelocity.Size(),
		0.f,
		1000.f,
		0.f,
		25000.f
	);
	GetMesh()->SetAllMotorsAngularDriveParams(OutVelocityValue,0.f,0.f,false);

	//防止Z方向的速度增加，穿过地板
	GetMesh()->SetEnableGravity(
		LastRagdollVelocity.Z > -4000.f
	);

	//更新布娃娃当前位置
	SetActorLocationDuringRagdoll();
}

void ARougeCharacter::SetActorLocationDuringRagdoll()
{
	//把Socket pelvis为布娃娃的目标Location
	TargetRagdollLocation = GetMesh()->GetSocketLocation(FName("pelvis"));

	//设置布娃娃的目标Rotation
	bRagdollFaceUp = GetMesh()->GetSocketRotation(FName("pelvis")).Roll < 0.f;
	float TargetRagdollRotationYaw = bRagdollFaceUp ? GetMesh()->GetSocketRotation(FName("pelvis")).Yaw - 180.f : GetMesh()->GetSocketRotation(FName("pelvis")).Yaw;
	TargetRagdollRotation = FRotator(0.f,TargetRagdollRotationYaw,0.f);

	//防止胶囊体一半穿过地面
	//从目标位置向下追踪以抵消目标位置，防止当布娃娃躺在地上时胶囊的下半部分穿过地板
	const FVector Start = TargetRagdollLocation;
	const float EndZ = TargetRagdollLocation.Z - GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	const FVector End = FVector(TargetRagdollLocation.X,TargetRagdollLocation.Y,EndZ);

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECollisionChannel::ECC_Visibility
	);

	bRagdollOnGround = HitResult.bBlockingHit;
	if(bRagdollOnGround)
	{
		float CapsuleLocationZ = GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - FMath::Abs(HitResult.ImpactPoint.Z - HitResult.TraceStart.Z);
		float NewLocationZ = TargetRagdollLocation.Z + CapsuleLocationZ + 2.f;
		FVector NewLocation = FVector(TargetRagdollLocation.X,TargetRagdollLocation.Y,NewLocationZ);

		SetActorLocationAndRotationFromTarget(NewLocation,TargetRagdollRotation,false, false);
	}else
	{
		SetActorLocationAndRotationFromTarget(TargetRagdollLocation,TargetRagdollRotation,false, false);
	}
}

void ARougeCharacter::RollTimerHandlerCallback()
{
	//八方向
	const FRotator ControlRotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	const FVector ControlForwardVector = UKismetMathLibrary::GetForwardVector(ControlRotation);
	const FVector ControlRightVector = UKismetMathLibrary::GetRightVector(ControlRotation);
	const FVector DirectionVector = MoveF * ControlForwardVector + MoveR * ControlRightVector;

	const FRotator DirectionRotator = DirectionVector.Rotation();
	DeltaDirectRotation = UKismetMathLibrary::NormalizedDeltaRotator(GetActorRotation(),DirectionRotator);

	if(FMath::Abs(MoveF) + FMath::Abs(MoveR) == 0.f)
	{
		ZYaw = 0.f;
	}else
	{
		ZYaw = DeltaDirectRotation.Yaw;
	}
}

void ARougeCharacter::SetActorLocationAndRotationFromTarget(FVector NewLocation,FRotator NewRotation,bool bSweep,bool bTeleport)
{
	CurrentTargetRotation = NewRotation;
	SetActorLocationAndRotation(
		NewLocation,
		NewRotation
	);
}

void ARougeCharacter::RagdollAction()
{
	switch (MovementState)
	{
	case EMovementState::EMS_None:
	case EMovementState::EMS_Grounded:
	case EMovementState::EMS_Mantiling:
	case EMovementState::EMS_InAir:
		RagdollStart();
		break;
	case EMovementState::EMS_RagDoll:
		RagdollEnd();
		break;
	}
}

void ARougeCharacter::RagdollStart()
{
	//禁止移动
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();

	GetCharacterMovement()->SetMovementMode(MOVE_None,0);
	MovementState = EMovementState::EMS_RagDoll;

	//取消碰撞
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionObjectType(ECC_PhysicsBody);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	const FName InBoneName = UKismetSystemLibrary::MakeLiteralName(FName("pelvis"));
	GetMesh()->SetAllBodiesBelowSimulatePhysics(InBoneName,true,true);

	//停止所有蒙太奇动画
	if(RougeDemoAnimInstance)
	{
		RougeDemoAnimInstance->Montage_Stop(0.2f);
	}
}

void ARougeCharacter::RagdollEnd()
{
	MovementState = EMovementState::EMS_Grounded;
	if(RougeDemoAnimInstance)
	{
		//保存当前姿势的快照
		RougeDemoAnimInstance->SavePoseSnapshot(FName("RagdollPose"));

		//检测当前布娃娃形态是否着地
		if(bRagdollOnGround)
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			UAnimMontage* GetUpAnim = GetGetUpAnimation();
			if(GetUpAnim)
			{
				RougeDemoAnimInstance->Montage_Play(GetUpAnim,1.f);
			}
		}else
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Falling);
			GetCharacterMovement()->Velocity = LastRagdollVelocity;
		}

		//重新启用胶囊碰撞，并在网格上禁用物理模拟
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetCollisionObjectType(ECC_Pawn);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		GetMesh()->SetAllBodiesSimulatePhysics(false);
	}
}

UAnimMontage* ARougeCharacter::GetGetUpAnimation()
{
	if(bRagdollFaceUp)
	{
		switch (OverlayState)
		{
		case EOverlayState::EOS_Default:
			return GetUpFrontDefault;
		default:
			UE_LOG(LogTemp,Warning,TEXT("Can't setting Getup Montage Anim"));
			return nullptr;
		}
	}else
	{
		switch (OverlayState)
		{
		case EOverlayState::EOS_Default:
			return GetUpBackDefault;
		default:
			UE_LOG(LogTemp,Warning,TEXT("Can't setting Getup Montage Anim"));
			return nullptr;
		}
	}
}

void ARougeCharacter::MultiTapInput(float ResetTime)
{
	
}

void ARougeCharacter::LockOnAction()
{
	if(LockOnComp)
	{
		LockOnComp->ToggleLockOn();
	}
}

void ARougeCharacter::UpdateDynamicMovementSettings()
{
	//根据DataTable中的数据设置属性
	CurrentMovementSettings = GetTargetMovementSettings();

	//设置MaxWalkSpeed
	float MaxWalkSpeed = 0.f;
	if(AllowedGait == EGait::EG_Walking)
	{
		MaxWalkSpeed = CurrentMovementSettings.WalkSpeed;
	}else if(AllowedGait == EGait::EG_Running)
	{
		MaxWalkSpeed = CurrentMovementSettings.RunSpeed;
	}else if(AllowedGait == EGait::EG_Sprinting)
	{
		MaxWalkSpeed = CurrentMovementSettings.SprintSpeed;
	}

	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;

	//设置MaxAcceleration,BrakingDecelerationWalking,GroundFraction
	FVector CurveValue = CurrentMovementSettings.MovementCurve->GetVectorValue(
		GetMappedSpeed()
	);

	GetCharacterMovement()->MaxAcceleration = CurveValue.X;
	GetCharacterMovement()->BrakingDecelerationWalking = CurveValue.Y;
	GetCharacterMovement()->GroundFriction = CurveValue.Z;
}

FMovementSettings ARougeCharacter::GetTargetMovementSettings()
{
	if(MovementData == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("No MovementData setting, please check it."))
		return FMovementSettings();
	}
	switch (RotationMode)
	{
	case ERotationMode::ERM_VelocityDirection:
		switch (Stance)
		{
		case EStance::ES_Standing:
			return MovementData->VelocityDirection.Standing;
		case EStance::ES_Crouching:
			return MovementData->VelocityDirection.Crouching;
		}
	case ERotationMode::ERM_LookingDirection:
		switch (Stance)
		{
		case EStance::ES_Standing:
			return MovementData->LookingDirection.Standing;
		case EStance::ES_Crouching:
			return MovementData->LookingDirection.Crouching;;
		}
	case ERotationMode::ERM_Aiming:
		switch (Stance)
		{
		case EStance::ES_Standing:
			return MovementData->Aiming.Standing;
		case EStance::ES_Crouching:
			return MovementData->Aiming.Crouching;;
		}
	default:
		UE_LOG(LogTemp,Error,TEXT("No Rotation setting, please check it."))
	}
	return FMovementSettings();
}

void ARougeCharacter::UpdateGroundedRotation(float DeltaTime)
{
	//当攻击时，转向攻击方向
	if(bSwingBeforeAttack)
	{
		//判断输入方向
		EMovementDirection InputDirection = CalculateInputDirection();
		
		switch (InputDirection)
		{
		case EMovementDirection::EMD_Forward:
			AttackYaw = GetControlRotation().Yaw;
			break;
		case EMovementDirection::EMD_ForwardRight:
			AttackYaw = GetControlRotation().Yaw + 45.f;
			break;
		case EMovementDirection::EMD_Right:
			AttackYaw = GetControlRotation().Yaw + 90.f;
			break;
		case EMovementDirection::EMD_BackwardRight:
			AttackYaw = GetControlRotation().Yaw + 135.f;
			break;
		case EMovementDirection::EMD_ForwardLeft:
			AttackYaw = GetControlRotation().Yaw - 45.f;
			break;
		case EMovementDirection::EMD_Left:
			AttackYaw = GetControlRotation().Yaw - 90.f;
			break;
		case EMovementDirection::EMD_BackwardLeft:
			AttackYaw = GetControlRotation().Yaw - 135.f;
			break;
		case EMovementDirection::EMD_Backward:
			AttackYaw = GetControlRotation().Yaw  + 180.f;
			break;
		}
		const FRotator YawRotation(0.f,AttackYaw,0.f);
		
		const float RotationRate = CalculateGroundedRotationRate();
		SmoothCharacterRotation(YawRotation,800.f,RotationRate);
	}
}

void ARougeCharacter::ReGenerateOverlap()
{
	//重新开启碰撞检测
	AWeapon* CurrentWeapon = CombatComp->GetCurrentWeapon();
	if(CurrentWeapon)
	{
		CurrentWeapon->GetAttackBox()->SetGenerateOverlapEvents(true);
	}
}

bool ARougeCharacter::CanUpdateMovingRotation()
{
	return ((bIsMoving && bHasMovementInput) || Speed > 150.f) && HasAnyRootMotion();
}

void ARougeCharacter::SmoothCharacterRotation(FRotator TargetRotation,float TargetInterpSpeed,float ActorInterpSpeed)
{
	//当前TargetRotation插值到TargetRotation
	CurrentTargetRotation = UKismetMathLibrary::RInterpTo_Constant(
		CurrentTargetRotation,
		TargetRotation,
		UGameplayStatics::GetWorldDeltaSeconds(GetWorld()),
		TargetInterpSpeed);

	//Actor的Rotation平滑到当前TargetRotation
	FRotator ActorRotation = UKismetMathLibrary::RInterpTo(
		GetActorRotation(),
		CurrentTargetRotation,
		UGameplayStatics::GetWorldDeltaSeconds(GetWorld()),
		ActorInterpSpeed);

	SetActorRotation(ActorRotation);
}

float ARougeCharacter::GetMappedSpeed()
{
	float LocWalkSpeed = CurrentMovementSettings.WalkSpeed;
	float LocRunSpeed = CurrentMovementSettings.RunSpeed;
	float LocSprintSpeed = CurrentMovementSettings.SprintSpeed;
	float WalkRange = UKismetMathLibrary::MapRangeClamped(
		Speed,
		0.f,
		LocWalkSpeed,
		0.f,
		1.f
	);
	float RunRange = UKismetMathLibrary::MapRangeClamped(
		Speed,
		LocWalkSpeed,
		LocRunSpeed,
		1.f,
		2.f
	);
	
	return Speed > LocWalkSpeed ? WalkRange : RunRange;;
}

float ARougeCharacter::CalculateGroundedRotationRate()
{
	if(CurrentMovementSettings.RotationRateCurve == nullptr) return 0.f;

	//获取曲线值
	float MappedSpeedValue = CurrentMovementSettings.RotationRateCurve->GetFloatValue(GetMappedSpeed());

	//对曲线值进行映射
	float RangeValue = UKismetMathLibrary::MapRangeClamped(
		AimYawRate,
		0.f,
		300.f,
		1.f,
		3.f
	);

	return MappedSpeedValue * RangeValue;
}

float ARougeCharacter::GetAnimCurveValue(FName CurveName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance)
	{
		return AnimInstance->GetCurveValue(CurveName);
	}else
	{
		return 0.f;
	}
}

void ARougeCharacter::SetMovementModel()
{
	if(MovementModelDataTable)
	{
		const FName RowName("Normal");
		const FString ContextString = TEXT("Attempting to find Skill_01 in SkillTable");
		MovementData = MovementModelDataTable->FindRow<FMovementSettingsState>(RowName,ContextString);
	}
}

void ARougeCharacter::UpdateCharacterMovement(float DeltaTime)
{
	//可执行的Gait状态
	AllowedGait = GetAllowedGait();

	//根据速度来判断在Gait中的哪个状态
	ActualGait = GetActualGait();
	//若不一致，则更新当前Gait，动画蓝图可调用
	if(ActualGait != Gait)
	{
		Gait = ActualGait;
	}

	//Use Movement Settings State
	UpdateDynamicMovementSettings();
}


EGait ARougeCharacter::GetActualGait()
{
	const float LocalWalkSpeed = CurrentMovementSettings.WalkSpeed;
	const float LocalRunSpeed = CurrentMovementSettings.RunSpeed;

	if(Speed >= LocalRunSpeed + 10.f)
	{
		switch (AllowedGait)
		{
		case EGait::EG_Walking:
		case EGait::EG_Running:
			return EGait::EG_Running;
		case EGait::EG_Sprinting:
			return EGait::EG_Sprinting;
		}
	}else if(Speed >= LocalWalkSpeed + 10.f)
	{
		return EGait::EG_Running;
	}else
	{
		return EGait::EG_Walking;
	}
	
	return EGait::EG_Walking;
}

EGait ARougeCharacter::GetAllowedGait()
{

	return EGait::EG_Walking;
}

// void ARougeCharacter::MoveForward(float Value)
// {
// 	if(Controller != nullptr && Value != 0.f)
// 	{
// 		const FRotator YawRotation(0.f,Controller->GetControlRotation().Yaw,0.f);
// 		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
// 		AddMovementInput(2 * Direction,Value);	 
// 	}
// 	MoveF = Value;
// 	//UE_LOG(LogTemp,Warning,TEXT("MoveF[%f]"),MoveF);
// }
//
// void ARougeCharacter::MoveRight(float Value)
// {
// 	if(Controller != nullptr && Value != 0.f)
// 	{
// 		const FRotator YawRotation(0.f,Controller->GetControlRotation().Yaw,0.f);
// 		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
// 		AddMovementInput(2 * Direction,Value);
// 	}
//
// 	MoveR = Value;
// }

void ARougeCharacter::AimOffset2Target(float DeltaTime, AActor* TargetActor)
{
	//LookDirection
	/*CurrentRotation = FRotator(0.f,GetBaseAimRotation().Yaw,0.f);
	AO_Yaw = UKismetMathLibrary::NormalizedDeltaRotator(CurrentRotation, StartRotation).Yaw;
	bUseControllerRotationYaw = false;*/
	
	//头朝向锁定目标
	if(TargetActor)
	{
		CurrentRotation = FRotator(0.f,UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),TargetActor->GetActorLocation()).Yaw,0.f);
		AO_Yaw = UKismetMathLibrary::NormalizedDeltaRotator(CurrentRotation, GetBaseAimRotation()).Yaw;
		AO_Pitch = GetBaseAimRotation().Pitch;
		bUseControllerRotationYaw = false;
	}
}

void ARougeCharacter::SetEssentialValues(float DeltaTime)
{
	//计算加速度
	Acceleration = CalculateAcceleration();

	//计算速度
	FVector Velocity = GetVelocity();
	Velocity.Z = 0;
	Speed = Velocity.Size();
	bIsMoving = Speed > 1.f;
	//是否移动
	if(bIsMoving)
	{
		//最后一帧的速度朝向
		LastVelocityRotation = FRotationMatrix::MakeFromX(GetVelocity()).Rotator();
	}

	//检测角色是否有输入，如果角色有移动输入，更新最后一次移动输入旋转
	MovementInputAmount = GetCharacterMovement()->GetCurrentAcceleration().Size() / GetCharacterMovement()->GetMaxAcceleration();
	bHasMovementInput = MovementInputAmount > 0.f;
	if(MovementInputAmount > 0.f)
	{
		LastMovementInputRotation = FRotationMatrix::MakeFromX(GetCharacterMovement()->GetCurrentAcceleration()).Rotator();
	}

	//相机的旋转速度
	AimYawRate = UKismetMathLibrary::Abs(GetControlRotation().Yaw - PreviousAimYaw / DeltaTime);

}

FVector ARougeCharacter::CalculateAcceleration()
{
	//根据速度公式 delta Velocity / delta time = acceleration
	return (GetVelocity() - PreviousVelocity) / UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
}

void ARougeCharacter::CacheValues(float DeltaTime)
{
	//上一帧的速度
	PreviousVelocity = GetVelocity();
	//上一帧的相机旋转角度
	PreviousAimYaw = GetControlRotation().Yaw;
}

void ARougeCharacter::AttackAction()
{
	if(CombatComp)
	{
		CombatComp->Attack();
	}
}

UAbilitySystemComponent* ARougeCharacter::GetAbilitySystemComponent() const
{
	return Cast<UAbilitySystemComponent>(PawnExtComponent->GetRougeAbilitySystemComponent());
}

URougeAbilitySystemComponent* ARougeCharacter::GetRougeAbilitySystemComponent() const
{
	return PawnExtComponent->GetRougeAbilitySystemComponent();
}

void ARougeCharacter::Elim()
{
	GetWorld()->GetTimerManager().SetTimer(
		ElimTimerHandle,
		this,
		&ARougeCharacter::ElimTimerFinished,
		ElimDelay,
		false
	);

}

void ARougeCharacter::ElimTimerFinished()
{
	ARougeGameMode* RougeDemoGameMode = GetWorld()->GetAuthGameMode<ARougeGameMode>();
	if(RougeDemoGameMode)
	{
		RougeDemoGameMode->RequetRespwan(this,Controller);
	}
}

// Called to bind functionality to input
void ARougeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PawnExtComponent->SetupPlayerInputComponent();
}

void ARougeCharacter::SetDisableInput(bool bNewDisableInput)
{
	bDisableInput = bNewDisableInput;
}





