// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enum/ECombatType.h"
#include "CombatComponent.generated.h"


class ABaseAI;
class AWeapon;
class ARougeDemoCharacter;
class UAnimMontage;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROUGEDEMO_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void Initialize();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWeapon> CurrentWeaponClass;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* FinisherMontage;
	
private:
	//是否在战斗中
	bool bIsInCombat = false;
	
	bool CheckAttackState();

	UPROPERTY()
	ARougeDemoCharacter* RougeDemoCharacter;

	void EquipWeapon();

	UPROPERTY()
	AWeapon* CurrentWeapon;

	ECombatType CombatType;

	UPROPERTY(VisibleAnywhere)
	ABaseAI* ExecutionEnemyRef;

	//处决触发
	void FinisherTimerTrigger();
	
	//处决开始
	void OnFinisherStart();

	bool IsIdleAndFalling();

	//bool ApplyRiosteEffect(float Duration,AActor* Applier,float Damage);
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Attack();

	//是否拔刀 true 已经拔刀 false 未拔刀
	bool bIsKatana;
};
