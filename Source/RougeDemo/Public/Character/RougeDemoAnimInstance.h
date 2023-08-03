// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enum/EMovementDirection.h"
#include "Enum/ERotationMode.h"
#include "Enum/EGait.h"
#include "Enum/EMovementState.h"
#include "Enum/EOverlayState.h"
#include "Enum/EStance.h"
#include "Struct/VelocityBlend.h"
#include "RougeDemoAnimInstance.generated.h"

class ARougeDemoCharacter;
class UCurveVector;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API URougeDemoAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
private:
	UPROPERTY(BlueprintReadOnly, Category=Character, meta=(AllowPrivateAccess = "true"))
	bool bIsAccelerating;

	UPROPERTY(BlueprintReadOnly, Category=Character, meta=(AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category=Character, meta=(AllowPrivateAccess = "true"))
	ARougeDemoCharacter* RougeDemoCharacter;

	UPROPERTY(BlueprintReadOnly, Category=Character, meta=(AllowPrivateAccess = "true"))
	float PitchOffset;

	UPROPERTY(BlueprintReadOnly, Category=Character, meta=(AllowPrivateAccess = "true"))
	float YawOffset;

	UPROPERTY(BlueprintReadOnly, Category=Character, meta=(AllowPrivateAccess = "true"))
	bool bIsAir;

	UPROPERTY(BlueprintReadOnly, Category=Character, meta=(AllowPrivateAccess = "true"))
	FVector Velocity;

	bool bIsMoving;

	UPROPERTY(BlueprintReadOnly, Category=Character, meta=(AllowPrivateAccess = "true"))
	bool bHasMovementInput;

	FVector MovementInput;

	UPROPERTY(BlueprintReadOnly, Category=AimingValue, meta=(AllowPrivateAccess = "true"))
	float InputYawOffsetTime;

	UPROPERTY(BlueprintReadOnly, Category=AimingValue, meta=(AllowPrivateAccess = "true"))
	float LeftYawTime;

	UPROPERTY(BlueprintReadOnly, Category=AimingValue, meta=(AllowPrivateAccess = "true"))
	float RightYawTime;

	UPROPERTY(BlueprintReadOnly, Category=AimingValue, meta=(AllowPrivateAccess = "true"))
	float ForwardYawTime;
	
	//Lean
	UPROPERTY(BlueprintReadOnly, Category=Character, meta=(AllowPrivateAccess = "true"))
	FVector2D LeanAmount;

	UPROPERTY(BlueprintReadOnly, Category=Character, meta=(AllowPrivateAccess = "true"))
	bool bShouldMove;
	
	FRotator AimingRotation;

	UPROPERTY(BlueprintReadOnly, Category=Character, meta=(AllowPrivateAccess = "true"))
	EOverlayState OverlayState;

	//移动姿态 Walking,Running,Sprinting
	UPROPERTY(BlueprintReadOnly, Category=Character, meta=(AllowPrivateAccess = "true"))
	EGait Gait;

	UPROPERTY(BlueprintReadOnly, Category=Character, meta=(AllowPrivateAccess = "true"))
	EMovementState MovementState;

	//暂时无用
	EMovementState PrevMovementState;

	UPROPERTY(BlueprintReadOnly, Category=Character, meta=(AllowPrivateAccess = "true"))
	ERotationMode RotationMode;

	UPROPERTY(BlueprintReadOnly, Category=Character, meta=(AllowPrivateAccess = "true"))
	EStance Stance;

	UPROPERTY(BlueprintReadOnly, Category=Character, meta=(AllowPrivateAccess = "true"))
	float Direction;
	
	//CurveValue
	UPROPERTY(BlueprintReadOnly, Category=CurveValue, meta=(AllowPrivateAccess = "true"))
	float BasePoseN;

	UPROPERTY(BlueprintReadOnly, Category=CurveValue, meta=(AllowPrivateAccess = "true"))
	float BasePoseCLF;

	UPROPERTY(BlueprintReadOnly, Category=CurveValue, meta=(AllowPrivateAccess = "true"))
	float EnableAimOffset;

	UPROPERTY(BlueprintReadOnly, Category=CurveValue, meta=(AllowPrivateAccess = "true"))
	float SpineAdd;

	UPROPERTY(BlueprintReadOnly, Category=CurveValue, meta=(AllowPrivateAccess = "true"))
	float HeadAdd;

	UPROPERTY(BlueprintReadOnly, Category=CurveValue, meta=(AllowPrivateAccess = "true"))
	float ArmLAdd;

	UPROPERTY(BlueprintReadOnly, Category=CurveValue, meta=(AllowPrivateAccess = "true"))
	float ArmRAdd;

	UPROPERTY(BlueprintReadOnly, Category=CurveValue, meta=(AllowPrivateAccess = "true"))
	float HandR;

	UPROPERTY(BlueprintReadOnly, Category=CurveValue, meta=(AllowPrivateAccess = "true"))
	float HandL;

	float EnableHandIKL;

	float EnableHandIKR;

	UPROPERTY(BlueprintReadOnly, Category=CurveValue, meta=(AllowPrivateAccess = "true"))
	float ArmLLS;

	UPROPERTY(BlueprintReadOnly, Category=CurveValue, meta=(AllowPrivateAccess = "true"))
	float ArmRLS;

	UPROPERTY(BlueprintReadOnly, Category=CurveValue, meta=(AllowPrivateAccess = "true"))
	float ArmLMS;

	UPROPERTY(BlueprintReadOnly, Category=CurveValue, meta=(AllowPrivateAccess = "true"))
	float ArmRMS;

	FRotator CharacterRotationLastFrame;
	FRotator CharacterRotation;
	FRotator DeltaRotation;

	FVector Acceleration;

	FVector RelativeAccelerationAmount;

	//Config
	float GroundedLeanInterpSpeed = 4.f;
	float VelocityBlendInterpSpeed = 12.f;
	float AnimatedWalkSpeed = 150.f;
	float AnimatedRunSpeed = 350.f;
	float SmoothedAimingRotationInterpSpeed = 10.f;
	float InputYawOffsetInterpSpeed = 8.f;
	float AnimatedCrouchSpeed = 150.f;
	
	//Grounded Movement
	UPROPERTY(BlueprintReadOnly, Category=AnimGround, meta=(AllowPrivateAccess = "true"))
	float WalkRunBlend;

	//[0,1] StrideBlend[0,1] 混合步伐，0代表步伐没动，0.5代表伸出半步，1代表1步
	UPROPERTY(BlueprintReadOnly, Category=AnimGround, meta=(AllowPrivateAccess = "true"))
	float StrideBlend;

	//站立idle->Walk->Run动画的播放速率
	UPROPERTY(BlueprintReadOnly, Category=AnimGround, meta=(AllowPrivateAccess = "true"))
	float StandingPlayRate = 1.f;

	//蹲伏idle->Walk动画的播放速率
	UPROPERTY(BlueprintReadOnly, Category=AnimGround, meta=(AllowPrivateAccess = "true"))
	float CrouchingPlayRate = 1.f;
	
	UPROPERTY(BlueprintReadOnly, Category=AnimGround, meta=(AllowPrivateAccess = "true"))
	FVelocityBlend VelocityBlend;

	UPROPERTY(BlueprintReadOnly, Category=AnimGround, meta=(AllowPrivateAccess = "true"))
	float FYaw;

	UPROPERTY(BlueprintReadOnly, Category=AnimGround, meta=(AllowPrivateAccess = "true"))
	float BYaw;

	UPROPERTY(BlueprintReadOnly, Category=AnimGround, meta=(AllowPrivateAccess = "true"))
	float LYaw;

	UPROPERTY(BlueprintReadOnly, Category=AnimGround, meta=(AllowPrivateAccess = "true"))
	float RYaw;

	//移动方向
	UPROPERTY(BlueprintReadOnly, Category=AnimGround, meta=(AllowPrivateAccess = "true"))
	EMovementDirection MovementDirection;

	//平滑旋转
	UPROPERTY(BlueprintReadOnly, Category=AnimGround, meta=(AllowPrivateAccess = "true"))
	FRotator SmoothedAimingRotation;

	UPROPERTY(BlueprintReadOnly, Category=AnimGround, meta=(AllowPrivateAccess = "true"))
	FVector2D SmoothedAimingAngle;
	
	//曲线
	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category=BlendCurve, meta=(AllowPrivateAccess = "true"))
	UCurveFloat* StrideBlendNWalkCurve;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category=BlendCurve, meta=(AllowPrivateAccess = "true"))
	UCurveFloat* StrideBlendNRunCurve;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category=BlendCurve, meta=(AllowPrivateAccess = "true"))
	UCurveVector* YawOffsetFBCurve;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category=BlendCurve, meta=(AllowPrivateAccess = "true"))
	UCurveVector* YawOffsetLRCurve;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category=Ragdoll, meta=(AllowPrivateAccess = "true"))
	float FlailRate;
private:
	void UpdateLayerValues(float DeltaTime);

	void UpdateCharacterInfo(float DeltaTime);
	
	void UpdateAimingValues(float DeltaTime);
	
	void UpdateRotationValues();

	void UpdateRagdollValues();
	
	FVector CalculateRelativeAccelerationAmount();

	EMovementDirection CalculateMovementDirection();
	
	void UpdateMovementValues(float DeltaTime);

	//计算StrideBlend,Stride步伐长度 返回值 0 代表姿势， 1代表移动姿势Walk/Run 最大步伐
	float CalculateStrideBlend();

	//获取曲线值 Min <= Func_curve(Name) + Bias <= Max
	float GetAnimCurveClamped(FName Name,float Bias,float ClampMin,float ClampMax);

	//计算Walk/Run 0代表Walk， 1代表Run/Sprint
	float CalculateWalkRunBlend();

	//通过将角色的速度除以动画速度来计算蹲伏播放率。这个值需要与站立播放率分开，以提高运动时从蹲到站的混合
	float CalculateCrouchingPlayRate();
	
	//计算VelocityBlend 此值表示Player在每个方向上的速度量
	FVelocityBlend CalculateVelocityBlend();

	//VelocityBlend插值进行平滑处理
	FVelocityBlend InterpVelocityBlend(FVelocityBlend Current,FVelocityBlend Target,float InterpSpeed,float DeltaTime);

	//Lean插值平滑处理
	FVector2D InterpLeanAmount(FVector2D Current, FVector2D Target, float InterpSpeed, float DeltaTime);

	//计算站立下的动画播放速度 0.f <= Speed / (config_WalkSpeed * StrideBlend * ComponentScale) <= 3.f
	float CalculateStandingPlayRate();

	//检查Character是否可以移动
	bool ShouldMoveCheck();

	EMovementDirection CalculateQuadrant(EMovementDirection Current,float FR_Threshold,float FL_Threshold,float BR_Threshold,float BL_Threshold,float Buffer,float Angle);

	bool AngleInRange(float Angle,float MinAngle,float MaxAngle,float Buffer,bool bIncreaseBuffer);
	
	FVector2D AimingAngle;
protected:
	
};
