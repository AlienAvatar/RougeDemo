// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ModularGameState.h"
#include "RougeGameState.generated.h"

class URougeAbilitySystemComponent;
class URougeExperienceManagerComponent;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API ARougeGameState : public AModularGameStateBase
{
	GENERATED_BODY()

public:
	ARougeGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of AActor interface
	
	//~IAbilitySystemInterface
	// virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~End of IAbilitySystemInterface

	UFUNCTION(BlueprintCallable, Category = "Rouge|GameState")
	URougeAbilitySystemComponent* GetRougeAbilitySystemComponent() const { return AbilitySystemComponent; }
private:
	UPROPERTY()
	TObjectPtr<URougeExperienceManagerComponent> ExperienceManagerComponent;

	// 被用来向当前的客户端同步游戏状态
	UPROPERTY(VisibleAnywhere, Category = "Rouge|GameState")
	TObjectPtr<URougeAbilitySystemComponent> AbilitySystemComponent;


};
