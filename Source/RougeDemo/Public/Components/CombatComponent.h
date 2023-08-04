// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enum/ECombatType.h"
#include "CombatComponent.generated.h"


class ABaseAI;
class AWeapon;
class ARougeDemoCharacter;

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

private:
	//是否在战斗中
	bool bIsInCombat = false;
	
	void Attack();

	bool AttackBoolDisabled();

	UPROPERTY()
	ARougeDemoCharacter* RougeDemoCharacter;

	void EquipWeapon();

	UPROPERTY()
	AWeapon* CurrentWeapon;

	ECombatType CombatType;

	//处决触发
	void FinisherTimerTrigger();

	UPROPERTY(VisibleAnywhere)
	ABaseAI* ExecutionEnemyRef;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
