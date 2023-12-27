// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enum/EActiveAbilities.h"
#include "Enum/EPassiveAbilities.h"
#include "AbilityComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROUGEDEMO_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAbilityComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* HammerFX;
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void SetStartingAbility();

	TMap<EActiveAbilities, int32> ActiveAbilitiesMap;

	TMap<EPassiveAbilities, int32> PassiveAbilitiesMap;

	int32 MaxAbilityLevel = 5;

	TMap<EActiveAbilities, EPassiveAbilities> EvolutionMap;

	TArray<EPassiveAbilities> EvolutionPassiveArr;
private:
	void LevelUpHammer();

	float HammerDamage = 0.5f;

	void GrantHammer(bool Cast);

	void PrepareHammer();

	FTimerHandle PrepareHammerTimerHandle;

	void PrepareHammerTimerHandleCallback();

	// Hammer CD
	float CalculateHammerCoolDown();

	TArray<FTimerHandle> ActiveTimer;

	float HammerRadius = 300.0f;
};
