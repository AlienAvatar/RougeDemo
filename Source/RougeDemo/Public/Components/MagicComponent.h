// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enum/EActiveAbilities.h"
#include "Enum/EPassiveAbilities.h"
#include "MagicComponent.generated.h"

class AProjectileBase;
class ABaseProjectile;
class ABaseExplosion;
class UNiagaraSystem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROUGEDEMO_API UMagicComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMagicComponent();
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

	UFUNCTION(BlueprintCallable)
	void LevelUpLightning();

	void LevelUpFrostBolt();

	void LevelUpAbilityDamage(bool PowerUp);

	void LevelUpMaxHealth(bool PowerUp);
	
	void LevelUpTimerReduction(bool PowerUp);
	
	void LevelUpWalkSpeed(bool PowerUp);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* HammerFX;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABaseExplosion> LightningClass;

	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* HitFX;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectileBase> FrostBoltClass;
private:
	//Hammer
	float HammerDamage = 0.5f;

	float HammerRadius = 300.0f;
	
	void GrantHammer(bool Cast);

	void PrepareHammer();

	void ExecuteHammer(TArray<FHitResult> Hits, float Damage, float Radius, APlayerController* Controller);

	FTimerHandle PrepareHammerTimerHandle;
	
	void PrepareHammerTimerHandleCallback();
	
	//Lightning
	float LightningDamage = 20.0f;
	
	float LightningRadius = 150.0f;
	
	void PrepareLightning();

	void ExecuteLightning(FVector TargetLocation, ACharacter* Instigator, float Damage, float Radius);

	void GrantLightning(bool Case);
	
	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* LightningSystem;
	
	FVector LastLightningLoc;
	
	FTimerHandle PrepareLightningTimerHandle;
	
	void PrepareLightningTimerHandleCallback();
	
	// Hammer CD
	float CalculateHammerCoolDown();

	TArray<FTimerHandle> ActiveTimerArr;

	//FrostBolt
	int FrostBoltIndex = 0;

	float FrostBoltDamage = 5.0f;
	
	void PrepareFrostBolt();

	void ExecuteFrostBolt(AActor* TargetActor, ACharacter* Instigator, float Damage);

	FTimerHandle PrepareFrostBoltTimerHandle;

	void PrepareFrostBoltTimerHandleCallback();

	void GrantFrostBolt(bool Cast);

	float AbilityDamageMultiplier = 1.0f;
	
	float AbilityCoolDownTimeMultiplier = 1.0f;
};



