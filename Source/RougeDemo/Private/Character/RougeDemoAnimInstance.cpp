// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RougeDemoAnimInstance.h"

#include "Character/RougeDemoCharacter.h"
#include "Curves/CurveVector.h"
#include "Enum/EGait.h"
#include "Enum/EMovementState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void URougeDemoAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	RougeDemoCharacter = Cast<ARougeDemoCharacter>(TryGetPawnOwner());
}

void URougeDemoAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(RougeDemoCharacter == nullptr)
	{
		RougeDemoCharacter = Cast<ARougeDemoCharacter>(TryGetPawnOwner());
	}

	if(RougeDemoCharacter == nullptr)
	{
		return;
	}

	/*CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = RougeDemoCharacter->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation,CharacterRotationLastFrame);
	const float Target = Delta.Yaw / DeltaSeconds;
	const float Interp = FMath::FInterpTo(Lean,Target,DeltaSeconds,6.f);
	Lean = FMath::Clamp(Interp,-90.f,90.f);*/

	//每帧进行更新
	UpdateLayerValues(DeltaSeconds);
	UpdateCharacterInfo(DeltaSeconds);
	UpdateAimingValues(DeltaSeconds);
	
	switch (MovementState)
	{
	case EMovementState::EMS_None:
		break;
	case EMovementState::EMS_Grounded:
		bShouldMove = ShouldMoveCheck();
		if(bShouldMove)
		{
			UpdateMovementValues(DeltaSeconds);
			UpdateRotationValues();
		}
		break;
	case EMovementState::EMS_InAir:
		break;
	case EMovementState::EMS_Mantiling:
		break;
	case EMovementState::EMS_RagDoll:
		UpdateRagdollValues();
		break;
	}
	
}

void URougeDemoAnimInstance::UpdateLayerValues(float DeltaTime)
{
	EnableAimOffset = UKismetMathLibrary::Lerp(1.f,0.f,GetCurveValue("Mask_AimOffset"));
	//曲线混合，当BasePoseN=1，BasePoseCLF=0，角色处于站立，反之蹲下
	BasePoseN = GetCurveValue("BasePose_N");
	BasePoseCLF = GetCurveValue("BasePose_CLF");
	
	SpineAdd = GetCurveValue("Layering_Spine_Add");
	HeadAdd = GetCurveValue("Layering_Head_Add");
	ArmLAdd = GetCurveValue("Layering_Arm_L_Add");
	ArmRAdd = GetCurveValue("Layering_Arm_R_Add");
	
	HandR = GetCurveValue("Layering_Hand_R");
	HandL = GetCurveValue("Layering_Hand_L");
	
	//本地空间转换为骨骼空间
	ArmLLS = GetCurveValue("Layering_Arm_L_LS");
	ArmLMS = 1 - UKismetMathLibrary::FFloor(ArmLLS);
	ArmRLS = GetCurveValue("Layering_Arm_R_LS");
	ArmRMS = 1 - UKismetMathLibrary::FFloor(ArmRLS);
}

void URougeDemoAnimInstance::UpdateCharacterInfo(float DeltaTime)
{
	bIsAccelerating = RougeDemoCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	bIsAir = RougeDemoCharacter->GetCharacterMovement()->IsFalling();
	PitchOffset = RougeDemoCharacter->GetAO_Pitch();
	YawOffset = RougeDemoCharacter->GetAO_Yaw();

	Velocity = RougeDemoCharacter->GetVelocity();
	Speed = RougeDemoCharacter->GetSpeed();
	Acceleration = RougeDemoCharacter->GetAcceleration();
	bIsMoving = RougeDemoCharacter->GetIsMoving();
	bHasMovementInput = RougeDemoCharacter->GetHasMovementInput();
	AimingRotation = RougeDemoCharacter->GetControlRotation();
	MovementInput = RougeDemoCharacter->GetCharacterMovement()->GetCurrentAcceleration();
	
	MovementState = RougeDemoCharacter->GetMovementState();
	Gait = RougeDemoCharacter->GetGait();
	PrevMovementState = RougeDemoCharacter->GetPrevMovementState();
	RotationMode = RougeDemoCharacter->GetRotationMode();
	Stance = RougeDemoCharacter->GetStance();
	OverlayState = RougeDemoCharacter->GetOverlayState();
}

void URougeDemoAnimInstance::UpdateAimingValues(float DeltaTime)
{
	//平滑处理角色旋转，即使镜头旋转速度比当前Actor Rotation快，也能平滑处理，不受当前角色的旋转变化影响
	SmoothedAimingRotation = UKismetMathLibrary::RInterpTo(SmoothedAimingRotation,AimingRotation,DeltaTime,SmoothedAimingRotationInterpSpeed);

	//计算AimingRotation和ActorRotation的差值，包括平滑差值
	const FRotator DeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(AimingRotation,RougeDemoCharacter->GetActorRotation());
	AimingAngle = FVector2D(DeltaRotator.Yaw,DeltaRotator.Pitch);
	const FRotator SmoothDeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(SmoothedAimingRotation,RougeDemoCharacter->GetActorRotation());
	SmoothedAimingAngle = FVector2D(SmoothDeltaRotator.Yaw,SmoothDeltaRotator.Pitch);

	//VelocityDirection
	if(RotationMode == ERotationMode::ERM_VelocityDirection)
	{
		if(bHasMovementInput)
		{
			const FRotator MoveInputRotator = FRotationMatrix::MakeFromX(MovementInput).Rotator(); // 使用MakeFromX函数并提供X方向来纠正倾斜得到的旋转
			const FRotator MoveInputDeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(MoveInputRotator,RougeDemoCharacter->GetActorRotation());
			float Target = UKismetMathLibrary::MapRangeClamped(
				MoveInputDeltaRotator.Yaw,
				-180.f,
				180.f,
				0.f,
				1.f
			);

			InputYawOffsetTime = UKismetMathLibrary::FInterpTo(
				InputYawOffsetTime,
				Target,
				InputYawOffsetInterpSpeed,
				DeltaTime
			);
		}
	}

	//将瞄准偏移的Yaw分成3次，左，中，右，分别用这3个值对瞄准偏移进行混合，既保证瞄准响应（相机移动），也能平滑过渡
	LeftYawTime = UKismetMathLibrary::MapRangeClamped(
		FMath::Abs(SmoothedAimingAngle.X),
		0.f,
		180.f,
		0.5f,
		0.f
	);
	RightYawTime = UKismetMathLibrary::MapRangeClamped(
		FMath::Abs(SmoothedAimingAngle.X),
		0.f,
		180.f,
		0.5f,
		1.f
	);
	//人物面朝前方
	ForwardYawTime = UKismetMathLibrary::MapRangeClamped(
		SmoothedAimingAngle.X,
		180.f,
		180.f,
		0.f,
		1.f
	);
}

void URougeDemoAnimInstance::UpdateRotationValues()
{
	if(YawOffsetFBCurve == nullptr || YawOffsetLRCurve == nullptr) return;
	MovementDirection = CalculateMovementDirection();
	
	//const FRotator VelocityRotator = FRotationMatrix::MakeFromX(Velocity).Rotator();
	const FRotator VelocityRotator = UKismetMathLibrary::MakeRotFromX(Velocity);
	const FRotator ControlRotator = RougeDemoCharacter->GetControlRotation();
	const float DeltaRotatorYaw = UKismetMathLibrary::NormalizedDeltaRotator(VelocityRotator,ControlRotator).Yaw;
	FYaw = YawOffsetFBCurve->GetVectorValue(DeltaRotatorYaw).X;
	BYaw = YawOffsetFBCurve->GetVectorValue(DeltaRotatorYaw).Y;
	LYaw = YawOffsetLRCurve->GetVectorValue(DeltaRotatorYaw).X;
	RYaw = YawOffsetLRCurve->GetVectorValue(DeltaRotatorYaw).Y;
}

void URougeDemoAnimInstance::UpdateRagdollValues()
{
	const float RagdollVelocity = GetOwningComponent()->GetPhysicsLinearVelocity(FName("root")).Size();
	
	FlailRate = UKismetMathLibrary::MapRangeClamped(
		RagdollVelocity,
		0.f,
		1000.f,
		0.f,
		1.f
	);
}

FVector URougeDemoAnimInstance::CalculateRelativeAccelerationAmount()
{
	if(RougeDemoCharacter == nullptr){ return FVector(); }

	const float MaxAcceleration = RougeDemoCharacter->GetCharacterMovement()->GetMaxAcceleration();
	FVector MaxAccelerationVector = UKismetMathLibrary::Vector_ClampSizeMax(
		Acceleration,
		MaxAcceleration
	);
	MaxAccelerationVector /=  MaxAcceleration;

	const FQuat Quaternion = RougeDemoCharacter->GetActorRotation().Quaternion();
	const FVector Result = UKismetMathLibrary::Quat_UnrotateVector(
		Quaternion,
		MaxAccelerationVector
	);
	return Result;
}

EMovementDirection URougeDemoAnimInstance::CalculateMovementDirection()
{
	EMovementDirection Result = EMovementDirection::EMD_Forward;
	/*switch (RotationMode)
	{
	case ERotationMode::ERM_VelocityDirection:
		Result = EMovementDirection::EMD_Forward;
		break;
	case ERotationMode::ERM_LookingDirection:
	case ERotationMode::ERM_Aiming:
		const FRotator VelocityXRotator = UKismetMathLibrary::MakeRotFromX(Velocity);
		const FRotator DeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(VelocityXRotator,AimingRotation);
		
		Result = CalculateQuadrant(MovementDirection,70.f,-70.f,110.f,-110.f,5.f,DeltaRotator.Yaw);
	}*/

	const FRotator VelocityXRotator = UKismetMathLibrary::MakeRotFromX(Velocity);
	const FRotator DeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(VelocityXRotator,AimingRotation);
		
	Result = CalculateQuadrant(MovementDirection,70.f,-70.f,110.f,-110.f,5.f,DeltaRotator.Yaw);
	return Result;
}

void URougeDemoAnimInstance::UpdateMovementValues(float DeltaTime)
{
	//设置速度方向
	VelocityBlend = InterpVelocityBlend(VelocityBlend,CalculateVelocityBlend(),VelocityBlendInterpSpeed,DeltaTime);

	//设置倾斜
	RelativeAccelerationAmount = CalculateRelativeAccelerationAmount();
	FVector2D TargetLeanAmount(RelativeAccelerationAmount.Y,RelativeAccelerationAmount.X);
	LeanAmount = InterpLeanAmount(LeanAmount,TargetLeanAmount,GroundedLeanInterpSpeed,DeltaTime);

	WalkRunBlend = CalculateWalkRunBlend();
	StrideBlend = CalculateStrideBlend();
	StandingPlayRate = CalculateStandingPlayRate();

	//Debug Message
	//UE_LOG(LogTemp,Warning,TEXT("StrideBlend[%f]"),StrideBlend);
	//UE_LOG(LogTemp,Warning,TEXT("StandingPlayRate[%f]"),StandingPlayRate);
	//UE_LOG(LogTemp,Warning,TEXT("EnableAimOffset[%f]"),EnableAimOffset);
	//UE_LOG(LogTemp,Warning,TEXT("WalkRunBlend[%f]"),WalkRunBlend);
	//UE_LOG(LogTemp,Warning,TEXT("VelocityBlend F[%f]"),VelocityBlend.F);
	//UE_LOG(LogTemp,Warning,TEXT("Gait[%s]"),*UEnum::GetValueAsString(Gait));
	//UE_LOG(LogTemp,Warning,TEXT("MovementState[%s]"),*UEnum::GetValueAsString(MovementState));
	//UE_LOG(LogTemp,Warning,TEXT("MovementDirection[%s]"),*UEnum::GetValueAsString(MovementDirection));
	//UE_LOG(LogTemp,Warning,TEXT("YawOffset[%f]"),YawOffset);
	//UE_LOG(LogTemp,Warning,TEXT("PitchOffset[%f]"),PitchOffset);
	//UE_LOG(LogTemp,Warning,TEXT("PitchOffset[%f]"),PitchOffset);
	//UE_LOG(LogTemp,Warning,TEXT("FlailRate[%f]"),FlailRate);
}

float URougeDemoAnimInstance::CalculateStrideBlend()
{
	if(StrideBlendNWalkCurve == nullptr || StrideBlendNRunCurve == nullptr) return -1.f;
	
	const float WalkCurveValue = StrideBlendNWalkCurve->GetFloatValue(Speed);
	const float RunCurveValue = StrideBlendNRunCurve->GetFloatValue(Speed);
	const float Alpha = GetAnimCurveClamped(TEXT("Weight_Gait"),-1.f,0.f,1.f);
	
	/*UE_LOG(LogTemp,Warning,TEXT("Alpha[%f]"),Alpha);
	UE_LOG(LogTemp,Warning,TEXT("WalkCurveValue[%f]"),WalkCurveValue);
	UE_LOG(LogTemp,Warning,TEXT("RunCurveValue[%f]"),RunCurveValue);
	float WeightGait = GetCurveValue(TEXT("Weight_Gait"));
	UE_LOG(LogTemp,Warning,TEXT("WeightGait[%f]"),WeightGait);*/
	
	const float Result = UKismetMathLibrary::Lerp(
		WalkCurveValue,
		RunCurveValue,
		Alpha
	);
	return Result;
}

float URougeDemoAnimInstance::GetAnimCurveClamped(FName Name, float Bias, float ClampMin, float ClampMax)
{
	return UKismetMathLibrary::Clamp(
			GetCurveValue(Name) + Bias,
			ClampMin,
			ClampMax
			);
}

float URougeDemoAnimInstance::CalculateWalkRunBlend()
{
	switch (Gait)
	{
	case EGait::EG_Walking:
		return 0.f;
	case EGait::EG_Running:
	case EGait::EG_Sprinting:
		return 1.f;
	default:
		UE_LOG(LogTemp,Error,TEXT("Gait not set value"))
	}
	return -1.f;
}

float URougeDemoAnimInstance::CalculateCrouchingPlayRate()
{
	const float SkeletonScaleZ = GetOwningComponent()->GetRelativeScale3D().Z;
	const float PlayRateValue = Speed / AnimatedCrouchSpeed / StrideBlend / SkeletonScaleZ;
	return UKismetMathLibrary::Clamp(PlayRateValue,0.f,2.f);
}

FVelocityBlend URougeDemoAnimInstance::CalculateVelocityBlend()
{
	FVelocityBlend ResultVelocityBlend;
	UKismetMathLibrary::Vector_Normalize(Velocity,0.1f);
	FQuat ActorRotationQuat = RougeDemoCharacter->GetActorRotation().Quaternion();
	//UnrotateVector 速度方向绕着原点（ActorLocation)逆时针旋转ActorRotation
	const FVector LocRelativeVelocityDir = UKismetMathLibrary::Quat_UnrotateVector(
		ActorRotationQuat,
		Velocity
	);
	
	const float Sum = LocRelativeVelocityDir.X + LocRelativeVelocityDir.Y + LocRelativeVelocityDir.Z;

	const FVector RelativeDirection = LocRelativeVelocityDir / Sum;
	//计算速度分量上的大小，0代表当前分量上没有速度，1代表分量上速度最大
	ResultVelocityBlend.F = UKismetMathLibrary::Clamp(RelativeDirection.X, 0.f, 1.f);
	ResultVelocityBlend.B = UKismetMathLibrary::Abs(UKismetMathLibrary::Clamp(RelativeDirection.X, -1.f, 0.f));
	ResultVelocityBlend.L = UKismetMathLibrary::Abs(UKismetMathLibrary::Clamp(RelativeDirection.Y, -1.f, 0.f));
	ResultVelocityBlend.R = UKismetMathLibrary::Clamp(RelativeDirection.X, 0.f, 1.f);
	return ResultVelocityBlend;
}

FVelocityBlend URougeDemoAnimInstance::InterpVelocityBlend(FVelocityBlend Current,FVelocityBlend Target,float InterpSpeed,float DeltaTime)
{
	FVelocityBlend ResultVelocityBlend;
	ResultVelocityBlend.F = UKismetMathLibrary::FInterpTo(Current.F,Target.F,DeltaTime,InterpSpeed);
	ResultVelocityBlend.B = UKismetMathLibrary::FInterpTo(Current.B,Target.B,DeltaTime,InterpSpeed);
	ResultVelocityBlend.L = UKismetMathLibrary::FInterpTo(Current.L,Target.L,DeltaTime,InterpSpeed);
	ResultVelocityBlend.R = UKismetMathLibrary::FInterpTo(Current.R,Target.R,DeltaTime,InterpSpeed);
	return ResultVelocityBlend;
}

FVector2D URougeDemoAnimInstance::InterpLeanAmount(FVector2D Current, FVector2D Target, float InterpSpeed, float DeltaTime)
{
	FVector2D ResultLeanAmount;
	ResultLeanAmount.X = UKismetMathLibrary::FInterpTo(Current.X, Target.X, InterpSpeed, DeltaTime);
	ResultLeanAmount.Y = UKismetMathLibrary::FInterpTo(Current.Y, Target.Y, InterpSpeed, DeltaTime);
	return ResultLeanAmount;
}

float URougeDemoAnimInstance::CalculateStandingPlayRate()
{
	float ResultPlayRate = 0.f;
	const float WalkSpeedRatio = Speed / AnimatedWalkSpeed;
	const float RunSpeedRatio = Speed / AnimatedRunSpeed;
	const float Alpha = GetAnimCurveClamped(TEXT("Weight_Gait"),-1.f,0.f,1.f);
	const float Ratio = UKismetMathLibrary::Lerp(
		WalkSpeedRatio,
		RunSpeedRatio,
		Alpha
	);
	const float ComponentZ = GetOwningComponent()->GetComponentScale().Z;
	ResultPlayRate =
		UKismetMathLibrary::Clamp(
		Ratio / StrideBlend / ComponentZ,
		0.f,
		3.f
	);
	return ResultPlayRate;
}

bool URougeDemoAnimInstance::ShouldMoveCheck()
{
	return (bIsMoving && bHasMovementInput) || Speed > 150.f;
}

EMovementDirection URougeDemoAnimInstance::CalculateQuadrant(EMovementDirection Current, float FR_Threshold, float FL_Threshold,
	float BR_Threshold, float BL_Threshold, float Buffer, float Angle)
{
	if(AngleInRange(Angle,FL_Threshold,FR_Threshold,Buffer,(Current != EMovementDirection::EMD_Forward || Current != EMovementDirection::EMD_Backward)))
	{
		return EMovementDirection::EMD_Forward;
	}else if(AngleInRange(Angle,FR_Threshold,BR_Threshold,Buffer,(Current != EMovementDirection::EMD_Right || Current != EMovementDirection::EMD_Left)))
	{
		return EMovementDirection::EMD_Right;
	}else if(AngleInRange(Angle,BL_Threshold,FL_Threshold,Buffer,(Current != EMovementDirection::EMD_Right || Current != EMovementDirection::EMD_Left)))
	{
		return EMovementDirection::EMD_Left;
	}else
	{
		return EMovementDirection::EMD_Backward;
	}
}

bool URougeDemoAnimInstance::AngleInRange(float Angle, float MinAngle, float MaxAngle, float Buffer,
	bool bIncreaseBuffer)
{
	bool bMinRange = UKismetMathLibrary::InRange_FloatFloat(
		Angle,
		MinAngle+Buffer,
		MaxAngle-Buffer,
		true,
		true
	);

	bool bMaxRange = UKismetMathLibrary::InRange_FloatFloat(
		Angle,
		MinAngle-Buffer,
		MaxAngle+Buffer,
		true,
		true
	);

	return bIncreaseBuffer ? bMaxRange : bMinRange;
}


