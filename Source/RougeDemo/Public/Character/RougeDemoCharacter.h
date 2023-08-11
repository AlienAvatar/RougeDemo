// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "Enum/EMovementState.h"
#include "Enum/EGait.h"
#include "Enum/EOverlayState.h"
#include "Enum/ERotationMode.h"
#include "Enum/EStance.h"
#include "Struct/MovementSettings.h"
#include "Struct/MovementSettingsState.h"
#include "RougeDemoCharacter.generated.h"

class URougeDemoAnimInstance;
class UCombatComponent;
class ULockOnComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class ROUGEDEMO_API ARougeDemoCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARougeDemoCharacter();


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

	void CacheValues(float DeltaTime);

	FVector PreviousVelocity;

	//加速度
	FVector Acceleration;

	EMovementState MovementState;

	bool bIsMoving;

	FRotator LastVelocityRotation;

	//[0,1]，0代表没有输入，1代表输入最大
	float MovementInputAmount;

	bool bHasMovementInput;

	FRotator LastMovementInputRotation;

	//初始化角色State
	EGait Gait = EGait::EG_Walking;
	EGait DesiredGait = EGait::EG_Running;
	EGait AllowedGait;
	EGait ActualGait;
	
	ERotationMode DesiredRotationMode = ERotationMode::ERM_VelocityDirection;
	ERotationMode RotationMode;

	EStance DesiredStance;
	EStance Stance = EStance::ES_Standing;
	//更新角色移动属性
	void UpdateCharacterMovement(float DeltaTime);

	//获取当前状态下可执行的Gait状态
	EGait GetAllowedGait();

	EGait GetActualGait();

	void UpdateDynamicMovementSettings();

	FMovementSettings GetTargetMovementSettings();
	
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

	//播放翻滚Root Motion Montage动画
	void RollEvent();

	//从Table中读取数据保存到Struct中
	void SetMovementModel();

	FMovementSettingsState* MovementData;

	EMovementState PrevMovementState;

	//每帧更新布娃娃的状态
	void RagdollUpdate(float DeltaTime);

	//最后一帧布娃娃速度
	FVector LastRagdollVelocity;

	void SetActorLocationDuringRagdoll();

	FVector TargetRagdollLocation;

	bool bRagdollFaceUp;

	FRotator TargetRagdollRotation;

	//检测布娃娃是否着地
	bool bRagdollOnGround;

	void SetActorLocationAndRotationFromTarget(FVector NewLocation,FRotator NewRotation,bool bSweep,bool bTeleport);

	void RagdollAction();

	void RagdollStart();

	void RagdollEnd();

	UAnimMontage* GetGetUpAnimation();

	//蹲伏事件
	void CrouchAction();

	//防止重复输入，每次输入都有间隔时间（CD)
	void MultiTapInput(float ResetTime);

	//锁敌事件
	void LockOnAction();

	//true 禁止玩家所有输入，false，玩家可以输入
	bool bDisableInput;

	virtual void AttackAction();

	FTimerHandle RollTimerHandle;

	void RollTimerHandlerCallback();
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
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Anim")
	UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* MovementModelDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category=GetUpAnimation)
	UAnimMontage* GetUpFrontDefault;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category=GetUpAnimation)
	UAnimMontage* GetUpBackDefault;

	EOverlayState OverlayState = EOverlayState::EOS_Default;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component)
	ULockOnComponent* LockOnComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component)
	UCombatComponent* CombatComp;

	float Speed;

	UPROPERTY()
	URougeDemoAnimInstance* RougeDemoAnimInstance;

	void StartSprint();

	void StopSprint();

	bool bIsSprint = false;

	void RollAction();

	UPROPERTY(EditDefaultsOnly,Category=MovementSystem)
	UAnimMontage* F_RollMontage;

	UPROPERTY(EditDefaultsOnly,Category=MovementSystem)
	UAnimMontage* B_RollMontage;

	UPROPERTY(EditDefaultsOnly,Category=MovementSystem)
	UAnimMontage* L_RollMontage;

	UPROPERTY(EditDefaultsOnly,Category=MovementSystem)
	UAnimMontage* R_RollMontage;

	float MoveF;

	float MoveR;

	FRotator DeltaDirectRotation;

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
	FORCEINLINE EMovementState GetPrevMovementState() const { return PrevMovementState; }
	FORCEINLINE EStance GetStance() const { return Stance; }
	FORCEINLINE EOverlayState GetOverlayState() const { return OverlayState; }
	FORCEINLINE ERotationMode GetRotationMode() const {return RotationMode; }
	ULockOnComponent* GetLockOnComp() const { return LockOnComp; }
	void SetDisableInput(bool bNewDisableInput);
	UCombatComponent* GetCombatComponent() const { return CombatComp; }
	URougeDemoAnimInstance* GetAnimationInstance() const { return RougeDemoAnimInstance; }

	//控制蒙太奇播放
	UPROPERTY(BlueprintReadWrite)
	bool bCanPlayMontage = true;
};
