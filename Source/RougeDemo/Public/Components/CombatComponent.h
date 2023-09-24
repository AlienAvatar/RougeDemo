// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enum/ECombatType.h"
#include "Enum/EMovementDirection.h"
#include "CombatComponent.generated.h"


class ABaseAI;
class AWeapon;
class ARougeDemoCharacter;
class UAnimMontage;
class APlayerKatanaCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROUGEDEMO_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();
	friend class ARougeDemoCharacter;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void Initialize();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWeapon> CurrentWeaponClass;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* FinisherMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* AttackMontage;

	UPROPERTY(BlueprintReadWrite)
	AWeapon* CurrentWeapon;
private:
	//是否在战斗中
	bool bIsInCombat = false;
	
	bool CheckAttackState();

	UPROPERTY()
	ARougeDemoCharacter* RougeDemoCharacter;

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

	UFUNCTION(BlueprintCallable)
	void Attack();

	UFUNCTION(BlueprintCallable)
	void SpawnWeapon();

	UFUNCTION(BlueprintCallable)
	void EquipWeapon();

	UFUNCTION(BlueprintCallable)
	void UnArmWeapon();

	FORCEINLINE AWeapon* GetCurrentWeapon() const { return CurrentWeapon; }
};
