// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "Input/RougeMappableConfigPair.h"
#include "RougeHeroComponent.generated.h"

/**
 * IGameFrameworkInitStateInterface 比如一个obj在初始化前必须要求另一个obj达到某个状态
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROUGEDEMO_API URougeHeroComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URougeHeroComponent(const FObjectInitializer& ObjectInitializer);

	//~ Begin IGameFrameworkInitStateInterface interface
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent);

	UPROPERTY(EditAnywhere)
	TArray<FMappableConfigPair> DefaultInputConfigs;
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
