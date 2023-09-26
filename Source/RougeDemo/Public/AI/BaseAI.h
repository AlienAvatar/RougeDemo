// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Enum/EAISense.h"
#include "GameFramework/Character.h"
#include "Struct/AttributeInfo.h"
#include "Enum/EState.h"
#include "Enum/EMovementDirection.h"
#include "Enum/EAIStage.h"
#include "Interface/EnemyAIInterface.h"
#include "BaseAI.generated.h"

class AAIEnemyController;
class USphereComponent;
class UAISense_Sight;
class UBaseAIAnimInstance;
class UWidgetComponent;
class UTimelineComponent;
class UAIPerceptionComponent;
class UAISenseConfig;
class UBoxComponent;
class UDataTable;

UCLASS()
class ROUGEDEMO_API ABaseAI : public ACharacter, public IEnemyAIInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseAI();

	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* TargetWidget;

	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* HealthWidget;

	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* ToughWidget;

	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* FinisherWidget;
	
	bool bCanBeExecuted = true;

	bool NormalEnemies;

	bool BossEnemies;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=State)
	FAttributeInfo AttributeInfo;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ToggleMarket(bool bLockOn);

	FORCEINLINE float GetSpeed() const { return Speed; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsAlive() const { return State != EState::ES_Dead; }

	UFUNCTION(BlueprintCallable)
	virtual bool DoMeleeAttack(float& Delay);

	UFUNCTION(BlueprintCallable)
	void ActivateLeftAttack();
	UFUNCTION(BlueprintCallable)
	void ActivateRightAttack();
	UFUNCTION(BlueprintCallable)
	void DeactivateLeftAttack();
	UFUNCTION(BlueprintCallable)
	void DeactivateRightAttack();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=State)
	EState State;

	UFUNCTION(BlueprintCallable)
	void ActivateRangeAttack();

	//播放远程攻击动画
	void PlayRangeAttackAnim();

	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(AActor* TargetActor);

	//攻击发射的Projectile
	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<AActor> ProjectileClass;

	//阶段
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EAIStage Stage;

	//获取当前状态
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EState GetCurrentState() const { return State; }

	//获取理想范围
	UFUNCTION(BlueprintCallable)
	virtual float GetIdealRange() override;

	FORCEINLINE UBaseAIAnimInstance* GetBaseAIAnimInstance() const { return BaseAIAnimInstance; }
protected:
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* LeftAttackSphere;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* RightAttackSphere;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	UBaseAIAnimInstance* BaseAIAnimInstance;
	
	UPROPERTY(BlueprintReadWrite)
	float Speed;

	UPROPERTY(BlueprintReadWrite)
	FVector Velocity;

	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController,AActor* DamageCauser);

	void UpdateHUDHealth();

	void UpdateHUDTough();
	
	FTimerHandle DestroyTimerHandle;
	
	//消除效果
	void Elim();
	
	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* DissolveTimeline;
	
	FOnTimelineFloat DissolveTrack;

	UPROPERTY(EditAnywhere)
	UCurveFloat* DissolveCurve;

	UFUNCTION()
	void UpdateDissolveMaterial(float DissolveValue);

	void StartDissolve();

	//更改在runtime
	UPROPERTY(VisibleAnywhere,Category=Elim)
	UMaterialInstanceDynamic* DynamicDissolveMaterialInstance;

	//从蓝图中设置
	UPROPERTY(EditAnywhere,Category=Elim)
	UMaterialInstance* DissolveMaterialInstance;

	void PlayElimMontage();

	UPROPERTY(EditAnywhere)
	UAnimMontage* F_ElimMontageRoot;

	//攻击
	UPROPERTY(EditAnywhere)
	UAnimMontage* AttackMontageRoot;

	//视觉感官配置文件
	UPROPERTY(EditAnywhere)
	UAISense_Sight* AISenseConfig;

	//伤害数值
	UPROPERTY(EditAnywhere)
	float DamageCount;

	UPROPERTY()
	AAIEnemyController* EnemyController;

	UPROPERTY(EditDefaultsOnly)
	float ToughRecoverAmount = 20.f;

	UPROPERTY(EditAnywhere)
	UAnimMontage* RangeAttackMontageRoot;

	virtual void UpdateGroundedRotation(float DeltaTime);

	UPROPERTY()
	AAIEnemyController* AIController;

	void SmoothCharacterRotation(FRotator Target,float TargetInterpSpeed,float ActorInterpSpeed);

	void UpdateCharacterInfo(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	bool CanSenseActor(AActor* Actor,EAISense Sense);

	virtual bool SetMovementSpeed(EGait Gait) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Attribute")
	UDataTable* AttributeDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Attribute")
	float ToughRecoverTimeRate = 0.75f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Attribute")
	FName AttributeRowName;
private:
	void Dead();

	void PlayHitReactMontage(EMovementDirection HitDirection);
	
	void DestroyCallBack();

	void SetHUDHealth();

	void SetHUDTough();
	
	bool CanUseAnyAbility();

	virtual float PlayAttackMeleeMontage();

	UFUNCTION()
	void OnLeftAttackBeginOverHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnRightAttackBeginOverHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FTimerHandle ToughRecoverTimer;

	void ToughRecoverTimerCallback();

	FRotator TargetRotation;

	//从DataTable中读取
	void SetAttributeInfo();

	bool bIsRecoveringToughness = false;

};
