// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enum/EMovementDirection.h"
#include "Enum/ERotationMode.h"
#include "Enum/EGait.h"
#include "Enum/EMovementState.h"
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

	EMovementState MovementState;

	bool bIsMoving;

	bool bHasMovementInput;
	
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

	//Lean
	UPROPERTY(BlueprintReadOnly, Category=Character, meta=(AllowPrivateAccess = "true"))
	FVector2D LeanAmount;

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
	//Grounded Movement
	UPROPERTY(BlueprintReadOnly, Category=AnimGround, meta=(AllowPrivateAccess = "true"))
	float WalkRunBlend;

	UPROPERTY(BlueprintReadOnly, Category=AnimGround, meta=(AllowPrivateAccess = "true"))
	float StrideBlend;

	UPROPERTY(BlueprintReadOnly, Category=AnimGround, meta=(AllowPrivateAccess = "true"))
	float StandingPlayRate = 1.f;

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

	UPROPERTY(BlueprintReadOnly, Category=AnimGround, meta=(AllowPrivateAccess = "true"))
	EMovementDirection MovementDirection;

	ERotationMode RotationMode;

	bool bShouldMove;
	//移动姿态
	EGait Gait;
	
	//曲线
	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category=BlendCurve, meta=(AllowPrivateAccess = "true"))
	UCurveFloat* StrideBlendNWalkCurve;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category=BlendCurve, meta=(AllowPrivateAccess = "true"))
	UCurveFloat* StrideBlendNRunCurve;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category=BlendCurve, meta=(AllowPrivateAccess = "true"))
	UCurveVector* YawOffsetFBCurve;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category=BlendCurve, meta=(AllowPrivateAccess = "true"))
	UCurveVector* YawOffsetLRCurve;
private:
	void UpdateLayerValues(float DeltaTime);

	void UpdateAimingValues();

	void UpdateRotationValues();
	
	FVector CalculateRelativeAccelerationAmount();

	EMovementDirection CalculateMovementDirection();
	
	void UpdateMovementValues(float DeltaTime);

	//计算StrideBlend,Stride步伐长度 返回值 0 代表姿势， 1代表移动姿势Walk/Run 最大步伐
	float CalculateStrideBlend();

	//获取曲线值 Min <= Func_curve(Name) + Bias <= Max
	float GetAnimCurveClamped(FName Name,float Bias,float ClampMin,float ClampMax);

	//计算Walk/Run 0代表Walk， 1代表Run/Sprint
	float CalculateWalkRunBlend();

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
protected:
	
};
