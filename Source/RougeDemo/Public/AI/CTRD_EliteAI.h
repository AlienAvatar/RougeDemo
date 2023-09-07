// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BaseAI.h"
#include "CTRD_EliteAI.generated.h"

class UCurveFloat;
class UTimelineComponent;
class UBlackboardComponent;
class UParticleSystem;
class UNiagaraSystem;
class UStaticMeshComponent;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API ACTRD_EliteAI : public ABaseAI
{
	GENERATED_BODY()

public:

	ACTRD_EliteAI();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void UpdateGroundedRotation(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	bool bLeft;

	UPROPERTY(EditDefaultsOnly)
	bool bRight;

	float YawOffset;

	float PitchOffset;

	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* RayAttackCurve;

	//射线发射时间段
	UPROPERTY()
	UTimelineComponent* RayAttackTimeLine;

	FOnTimelineFloat RayAttackTrack;

	UFUNCTION()
	void UpdateRayAttack(float NewPitchValue);
	UPROPERTY()
	TEnumAsByte<ETimelineDirection::Type> TimelineDirection; 

	UPROPERTY(EditDefaultsOnly)
	float CurveLenTime = 1.f;

	UPROPERTY(EditDefaultsOnly,Category="Attack")
	UNiagaraSystem* RayAttackFX;
	
	UPROPERTY(EditDefaultsOnly,Category="Attack")
	UNiagaraSystem* RayAttackHitFX;

	UPROPERTY(EditDefaultsOnly,Category="Attack")
	float RayAttackDamageCount = 10.f;

	UPROPERTY(EditDefaultsOnly,Category="Components")
	UStaticMeshComponent* RightLaser;

	UPROPERTY(EditDefaultsOnly,Category="Components")
	UStaticMeshComponent* LeftLaser;
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,FVector NormalImpulse,const FHitResult& Hit);
private:
	UFUNCTION()
	void OnTimelineEndEvent(float Output);

	//是否使用射线攻击
	bool bIsRayAttacking;
	
	void CacheValues(float DeltaTime);

	float AimYawRate;
	float PreviousAimYaw;

	void PlayRayAttackFX(FVector Location,FRotator Rotation,FHitResult HitResult);

	void CheckRayTrack(FHitResult HitResult);

	FTimerHandle MoveRayAttackTimerHandle;

	void MoveRayAttackHandleCallBack();
public:

	FORCEINLINE float GetYawOffset() const { return YawOffset; }
	FORCEINLINE float GetPitchOffset() const { return PitchOffset; }

	//射线攻击
	UFUNCTION(BlueprintCallable)
	void RayAttack();

	UPROPERTY()
	UBlackboardComponent* BlackboardComponent;

	FORCEINLINE float GetAimYawRate() const { return AimYawRate; }
};
