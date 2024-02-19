// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkComponent.h"
#include "RougeHealthComponent.generated.h"

class URougeAbilitySystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRougeHealth_DeathEvent, AActor*, OwningActor);
/**
 * ERougeDeathState
 *
 *	定义当前的死亡状态
 */
UENUM(BlueprintType)
enum class ERougeDeathState : uint8
{
	NotDead = 0,
	DeathStarted,
	DeathFinished
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROUGEDEMO_API URougeHealthComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URougeHealthComponent(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable)
	FRougeHealth_DeathEvent OnDeathStarted;

	UPROPERTY(BlueprintAssignable)
	FRougeHealth_DeathEvent OnDeathFinished;

	UFUNCTION(BlueprintPure, Category = "Rouge|Health")
	static URougeHealthComponent* FindHealthComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<URougeHealthComponent>() : nullptr); }

	UFUNCTION(BlueprintCallable, Category = "Rouge|Health")
	ERougeDeathState GetDeathState() const { return DeathState; }

	// Begins the death sequence for the owner.
	virtual void StartDeath();

	// Ends the death sequence for the owner.
	virtual void FinishDeath();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(ReplicatedUsing = OnRep_DeathState)
	ERougeDeathState DeathState;

	UFUNCTION()
	virtual void OnRep_DeathState(ERougeDeathState OldDeathState);

protected:
	UPROPERTY()
	TObjectPtr<URougeAbilitySystemComponent> AbilitySystemComponent;
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
