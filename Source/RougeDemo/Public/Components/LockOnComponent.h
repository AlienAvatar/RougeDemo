// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockOnComponent.generated.h"

class ULockOnWidget;
class ARougeDemoCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROUGEDEMO_API ULockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULockOnComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void Initialize();
	
	//选定最近的目标
	void SelectTarget();

	//通过Sphere Trace判断附件是否有目标体
	TArray<FHitResult> SearchTargets();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MarketWidgetClass;

	UPROPERTY()
	ULockOnWidget* MarketWidget;
	
	void ActivateLockOn();

	void DeactivateLockOn();

	bool bIsLockOn = false;

	UPROPERTY()
	AActor* LockOnTarget;

	UPROPERTY()
	ARougeDemoCharacter* PlayerRougeDemoCharacter;

	//查找范围，目标能够锁定的最远距离
	UPROPERTY(EditDefaultsOnly)
	float RadarRange = 5000.f;

	float FrontTargetDistance;

	float BackTargetDistance;
	
	//范围内的所有敌人
	UPROPERTY()
	TArray<AActor*> Enemies;

	AActor* IdentifyTeam(FHitResult Hit);

	//默认为前方优先级
	bool bHasFrontPriority = true;

	UPROPERTY()
	AActor* TempFrontTarget;

	UPROPERTY()
	AActor* TempBackTarget;

	//一直面向敌人
	void SetControlRotationTowardTarget();

	void ActivateLockOnScreenPositionBased();

	FVector2D ScreenCenterPosition;

	UPROPERTY()
	AActor* TempScreenTarget;

	//Debug Point
	void DrawPoint(FVector2D Position, FLinearColor Color, float Size);

	//Clear Market
	void ClearMarket(AActor* AIActor,bool bLockOn);

	float RotationThickness = 500.f;

	FTimerHandle ExistTargetTimer;

	void ExistTarget();
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//是否自动锁敌
	UPROPERTY(EditDefaultsOnly)
	bool bIsAutoTargeting = false;

	void ToggleLockOn();
};
