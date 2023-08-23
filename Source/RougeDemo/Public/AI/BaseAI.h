// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "Struct/AttributeInfo.h"
#include "Enum/EState.h"
#include "BaseAI.generated.h"

class UWidgetComponent;
class UTimelineComponent;
UCLASS()
class ROUGEDEMO_API ABaseAI : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseAI();

	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* TargetWidget;

	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* HealthWidget;
	
	bool bCanBeExecuted = true;

	bool NormalEnemies;

	bool BossEnemies;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=State)
	FAttributeInfo AttributeInfo;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ToggleMarket(bool bLockOn);

	FORCEINLINE float GetSpeed() const { return Speed; }


	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=State)
	EState State;

	UPROPERTY(BlueprintReadWrite)
	float Speed;

	UPROPERTY(BlueprintReadWrite)
	FVector Velocity;

	UPROPERTY(EditDefaultsOnly, Category=Hit)
	UAnimMontage* F_HitMontage;

	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController,AActor* DamageCauser);

	void UpdateHealthHUD();

	void SetHealthHUD(float NewHealth);

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

private:
	//受击
	virtual float OnTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, AActor* DamageCauser, AController* InstigatedByController, AActor* DamageCauserActor);

	virtual float InternalTakePointDamage(float Damage, FPointDamageEvent const& PointDamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	void Dead();

	void PlayHitReactMontage();
	
	void DestroyCallBack();
};
