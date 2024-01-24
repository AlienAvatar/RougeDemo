// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Components/AbilityComponent.h"
#include "RougeAbilitySystemComponent.generated.h"

/**
 * Rouge技能系统组件（ URougeAbilitySystemComponent )）扩展了 技能系统组件（ UAbilitySystemComponent ）功能
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROUGEDEMO_API URougeAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URougeAbilitySystemComponent();

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void TryActivateAbilitiesOnSpawn();
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

};
