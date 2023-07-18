// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RougeDemoCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

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
	//Gait = EGait::EG_Running;
	RotationMode = ERotationMode::ERM_VelocityDirection;
	DesiredGait = EGait::EG_Running;
}

// Called when the game starts or when spawned
void ARougeDemoCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


void ARougeDemoCharacter::UpdateDynamicMovementSettings()
{
	
}

void ARougeDemoCharacter::UpdateGroundedRotation(float DeltaTime)
{
	if(CanUpdateMovingRotation())
	{
		FRotator Target (0.f, LastVelocityRotation.Yaw, 0.f);
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
		float RotationAmountValue = GetAnimCurveValue(TEXT("RotationAmount"));
		if(UKismetMathLibrary::Abs(RotationAmountValue) > 0.001f)
		{
			float DeletaRotationYaw = RotationAmountValue * (UGameplayStatics::GetWorldDeltaSeconds(GetWorld()) / (1.f / 30.f));
			FRotator DeletaRotation (0.f, DeletaRotationYaw, 0.f);
			AddActorWorldRotation(DeletaRotation);
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

	float MappedSpeedValue = CurrentMovementSettings.RotationRateCurve->GetFloatValue(GetMappedSpeed());

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

// Called every frame
void ARougeDemoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//AimOffset(DeltaTime);
	SetEssentialValues(DeltaTime);
	switch (MovementState)
	{
	case EMovementState::EMS_Grounded:
		UpdateCharacterMovement(DeltaTime);
		UpdateGroundedRotation(DeltaTime);
		break;
	case EMovementState::EMS_InAir:
		break;
	case EMovementState::EMS_RagDoll:
		break;
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
		case EGait::ERM_Sprinting:
			return EGait::ERM_Sprinting;
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

void ARougeDemoCharacter::OnBeginPlay()
{
	//确保动画更新在Character后
	GetMesh()->AddTickPrerequisiteActor(this);

	//设置默认初始值
	TargetRotation = GetActorRotation();
	LastVelocityRotation = GetActorRotation();
	LastMovementInputRotation = GetActorRotation();
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

void ARougeDemoCharacter::CacheValues()
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

