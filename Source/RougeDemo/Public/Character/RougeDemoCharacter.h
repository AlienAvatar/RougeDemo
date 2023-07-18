// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enum/EMovementState.h"
#include "Enum/EGait.h"
#include "Enum/ERotationMode.h"
#include "Struct/MovementSettings.h"
#include "RougeDemoCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class ROUGEDEMO_API ARougeDemoCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARougeDemoCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void OnBeginPlay();

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=MovementSystem)
	FMovementSettings CurrentMovementSettings;
private:

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* FollowCamera;

	float AO_Pitch;
	float AO_Yaw;

	FRotator CurrentRotation;
	FRotator StartRotation;
	void AimOffset(float DeltaTime);

	void SetEssentialValues(float DeltaTime);
	
	FVector CalculateAcceleration();

	void CacheValues();

	FVector PreviousVelocity;

	//加速度
	FVector Acceleration;

	EMovementState MovementState;

	bool bIsMoving;

	float Speed;

	FRotator LastVelocityRotation;

	//[0,1]，0代表没有输入，1代表输入最大
	float MovementInputAmount;

	bool bHasMovementInput;

	FRotator LastMovementInputRotation;

	EGait Gait;
	EGait DesiredGait;
	EGait AllowedGait;
	EGait ActualGait;

	//更新角色移动属性
	void UpdateCharacterMovement(float DeltaTime);

	//获取当前状态下可执行的Gait状态
	EGait GetAllowedGait();

	ERotationMode RotationMode;

	EGait GetActualGait();

	void UpdateDynamicMovementSettings();

	void UpdateGroundedRotation(float DeltaTime);

	bool CanUpdateMovingRotation();

	//旋转用插值做平滑处理
	void SmoothCharacterRotation(FRotator Target,float TargetInterpSpeed,float ActorInterpSpeed);

	FRotator TargetRotation;

	//映射当前速度，0代表停止，1代表走路，2代表跑步，3代表冲刺
	float GetMappedSpeed();

	//计算旋转速率
	float CalculateGroundedRotationRate();

	//用来缓存上一帧的AimYaw
	float PreviousAimYaw;

	//Yaw旋转的速率，代表相机的旋转速度
	float AimYawRate;

	float GetAnimCurveValue(FName CurveName);

	FRotator AimingRotation;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE FVector GetAcceleration() const { return Acceleration; }
	FORCEINLINE EMovementState GetMovementState() const { return MovementState; }
	FORCEINLINE bool GetIsMoving() const { return bIsMoving; }
	FORCEINLINE bool GetHasMovementInput() const { return bHasMovementInput; }
	FORCEINLINE EGait GetGait() const { return Gait; }
	FORCEINLINE float GetSpeed() const { return Speed; }
	FORCEINLINE FRotator GetAimingRotation() const { return AimingRotation; }
};
