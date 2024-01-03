// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enum/EActiveAbilities.h"
#include "Enum/EPassiveAbilities.h"
#include "AbilityComponent.generated.h"

class UNiagaraSystem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROUGEDEMO_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAbilityComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	void SetStartingAbility();

	TMap<EActiveAbilities, int32> ActiveAbilitiesMap;

	TMap<EPassiveAbilities, int32> PassiveAbilitiesMap;

	int32 MaxAbilityLevel = 5;

	TMap<EActiveAbilities, EPassiveAbilities> EvolutionMap;

	TArray<EPassiveAbilities> EvolutionPassiveArr;

	void LevelUpHammer();

	void RefreshAbilities();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* HammerFX;

private:
	float HammerDamage = 0.5f;

	void GrantHammer(bool Cast);

	void PrepareHammer();

	FTimerHandle PrepareHammerTimerHandle;

	void PrepareHammerTimerHandleCallback();

	// Hammer CD
	float CalculateHammerCoolDown();

	TArray<FTimerHandle> ActiveTimerArr;

	float HammerRadius = 300.0f;


};
