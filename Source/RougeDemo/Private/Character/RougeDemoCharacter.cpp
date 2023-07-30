// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RougeDemoCharacter.h"

#include "Camera/CameraComponent.h"
#include "Character/RougeDemoAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Curves/CurveVector.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ARougeDemoCharacter::ARougeDemoCharacter()
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

	MovementState = EMovementState::EMS_Grounded;
}

// Called when the game starts or when spawned
void ARougeDemoCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnBeginPlay();
}

void ARougeDemoCharacter::OnBeginPlay()
{
	//确保动画更新在Character后
	GetMesh()->AddTickPrerequisiteActor(this);

	//设置默认状态
	Gait = DesiredGait;
	RotationMode = DesiredRotationMode;

	switch (DesiredStance)
	{
	case EStance::ES_Standing:
		UnCrouch();
		break;
	case EStance::ES_Crouching:
		Crouch();
		break;
	}

	//读取DataTable
	SetMovementModel();
	
	//设置默认初始值
	TargetRotation = GetActorRotation();
	LastVelocityRotation = GetActorRotation();
	LastMovementInputRotation = GetActorRotation();
}

// Called every frame
void ARougeDemoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//AimOffset(DeltaTime);
	
	SetEssentialValues(DeltaTime);
	CacheValues(DeltaTime);
	
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
	}
}

void ARougeDemoCharacter::RagdollUpdate(float DeltaTime)
{
	LastRagdollVelocity = GetMesh()->GetPhysicsLinearVelocity(FName("root"));

	//使用布娃娃的速度来缩放布娃娃骨骼关节
	float OutVelocityValue = UKismetMathLibrary::MapRangeClamped(
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

void ARougeDemoCharacter::SetActorLocationDuringRagdoll()
{
	//把Socket pelvis为布娃娃的目标Location
	TargetRagdollLocation = GetMesh()->GetSocketLocation(FName("pelvis"));

	//设置布娃娃的目标Rotation
	RagdollFaceUp = GetMesh()->GetSocketLocation(FName("pelvis")).X < 0;
	float TargetRagdollRotationYaw = RagdollFaceUp ? GetMesh()->GetSocketLocation(FName("pelvis")).Z - 180.f : GetMesh()->GetSocketLocation(FName("pelvis")).Z;
	TargetRagdollRotation = FRotator(0.f,TargetRagdollRotationYaw,0.f);

	//防止胶囊体一半穿过地面
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

	RagdollOnGround = HitResult.bBlockingHit;
	if(RagdollOnGround)
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

void ARougeDemoCharacter::SetActorLocationAndRotationFromTarget(FVector NewLocation,FRotator NewRotation,bool bSweep,bool bTeleport)
{
	TargetRotation = NewRotation;
	SetActorLocationAndRotation(
		NewLocation,
		NewRotation
	);
}

void ARougeDemoCharacter::UpdateDynamicMovementSettings()
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
	GetCharacterMovement()->MaxWalkSpeedCrouched = MaxWalkSpeed;

	//设置MaxAcceleration,BrakingDecelerationWalking,GroundFraction
	FVector CurveValue = CurrentMovementSettings.MovementCurve->GetVectorValue(
		GetMappedSpeed()
	);

	GetCharacterMovement()->MaxAcceleration = CurveValue.X;
	GetCharacterMovement()->BrakingDecelerationWalking = CurveValue.Y;
	GetCharacterMovement()->GroundFriction = CurveValue.Z;
}

FMovementSettings ARougeDemoCharacter::GetTargetMovementSettings()
{
	if(MovementData == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("No MovementData setting, please check it."))
		FMovementSettings();
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

void ARougeDemoCharacter::UpdateGroundedRotation(float DeltaTime)
{
	if(CanUpdateMovingRotation())
	{
		const FRotator Target (0.f, LastVelocityRotation.Yaw, 0.f);
		SmoothCharacterRotation(Target,800.f,CalculateGroundedRotationRate());
		/*switch (RotationMode)
		{
		case ERotationMode::ERM_VelocityDirection:
			
			
			break;
		case ERotationMode::ERM_LookingDirection:
			break;
		case ERotationMode::ERM_Aiming:
			break;
		}*/
	}else
	{
		const float RotationAmountValue = GetAnimCurveValue(TEXT("RotationAmount"));
		if(UKismetMathLibrary::Abs(RotationAmountValue) > 0.001f)
		{
			const float DeltaRotationYaw = RotationAmountValue * (UGameplayStatics::GetWorldDeltaSeconds(GetWorld()) / (1.f / 30.f));
			const FRotator DeltaRotation (0.f, DeltaRotationYaw, 0.f);
			AddActorWorldRotation(DeltaRotation);
			TargetRotation = GetActorRotation();
		}
	}
}

bool ARougeDemoCharacter::CanUpdateMovingRotation()
{
	return ((bIsMoving && bHasMovementInput) || Speed > 150.f) && HasAnyRootMotion();
}

void ARougeDemoCharacter::SmoothCharacterRotation(FRotator Target,float TargetInterpSpeed,float ActorInterpSpeed)
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

float ARougeDemoCharacter::GetMappedSpeed()
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
	
	return  Speed > LocWalkSpeed ? WalkRange : RunRange;;
}

float ARougeDemoCharacter::CalculateGroundedRotationRate()
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

float ARougeDemoCharacter::GetAnimCurveValue(FName CurveName)
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

void ARougeDemoCharacter::RollEvent()
{
	URougeDemoAnimInstance* RougeDemoAnimInstance = Cast<URougeDemoAnimInstance>(GetMesh()->GetAnimInstance());
	if(RougeDemoAnimInstance && AnimMontage)
	{
		RougeDemoAnimInstance->Montage_Play(AnimMontage,1.15);
	}
}

void ARougeDemoCharacter::SetMovementModel()
{
	if(MovementModelDataTable)
	{
		const FName RowName("Normal");
		const FString ContextString = TEXT("Attempting to find Skill_01 in SkillTable");
		MovementData = MovementModelDataTable->FindRow<FMovementSettingsState>(RowName,ContextString);
	}
}

void ARougeDemoCharacter::UpdateCharacterMovement(float DeltaTime)
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


EGait ARougeDemoCharacter::GetActualGait()
{
	float LocalWalkSpeed = CurrentMovementSettings.WalkSpeed;
	float LocalRunSpeed = CurrentMovementSettings.RunSpeed;
	float LocalSprintSpeed = CurrentMovementSettings.SprintSpeed;

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

EGait ARougeDemoCharacter::GetAllowedGait()
{
	if(RotationMode == ERotationMode::ERM_VelocityDirection || RotationMode == ERotationMode::ERM_LookingDirection)
	{
		switch (DesiredGait)
		{
		case EGait::EG_Walking:
			return EGait::EG_Walking;
		case EGait::EG_Running:
			return EGait::EG_Running;
		}
	}
	return EGait::EG_Walking;
}

void ARougeDemoCharacter::MoveForward(float Value)
{
	if(Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f,Controller->GetControlRotation().Yaw,0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(2 * Direction,Value);	 
	}
}

void ARougeDemoCharacter::MoveRight(float Value)
{
	if(Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f,Controller->GetControlRotation().Yaw,0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(2 * Direction,Value);
	}
}

void ARougeDemoCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ARougeDemoCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}



void ARougeDemoCharacter::AimOffset(float DeltaTime)
{
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();
	if(Speed == 0.f)
	{
		//LookDirection
		CurrentRotation = FRotator(0.f,GetBaseAimRotation().Yaw,0.f);
		AO_Yaw = UKismetMathLibrary::NormalizedDeltaRotator(CurrentRotation, StartRotation).Yaw;
		bUseControllerRotationYaw = false;
	}
	//runing
	if(Speed > 0.f)
	{
		StartRotation = FRotator(0.f,GetBaseAimRotation().Yaw,0.f);
		AO_Yaw = 0.f;
		bUseControllerRotationYaw = true;
	}
	AO_Pitch = GetBaseAimRotation().Pitch;
}

void ARougeDemoCharacter::SetEssentialValues(float DeltaTime)
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

FVector ARougeDemoCharacter::CalculateAcceleration()
{
	//根据速度公式 delta Velocity / delta time = acceleration
	return (GetVelocity() - PreviousVelocity) / UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
}

void ARougeDemoCharacter::CacheValues(float DeltaTime)
{
	//上一帧的速度
	PreviousVelocity = GetVelocity();
	//上一帧的相机旋转角度
	PreviousAimYaw = GetControlRotation().Yaw;
}


// Called to bind functionality to input
void ARougeDemoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&ARougeDemoCharacter::Jump);
	
	PlayerInputComponent->BindAxis("MoveForward",this,&ARougeDemoCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&ARougeDemoCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn",this,&ARougeDemoCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp",this,&ARougeDemoCharacter::LookUp);
}

