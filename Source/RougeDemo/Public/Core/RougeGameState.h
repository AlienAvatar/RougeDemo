// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ModularGameState.h"
#include "RougeGameState.generated.h"

class URougeAbilitySystemComponent;
class URougeExperienceManagerComponent;
/**
 * 高级游戏逻辑由 Rouge游戏状态（ARougeGameState）类在服务器端进行管理
 */
UCLASS()
class ROUGEDEMO_API ARougeGameState : public AModularGameStateBase
{
	GENERATED_BODY()

public:
	ARougeGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~AGameStateBase interface
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;
	//~End of AGameStateBase interface
	
	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of AActor interface

	float GetServerFPS() const { return ServerFPS; }
	
	//~IAbilitySystemInterface
	// virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~End of IAbilitySystemInterface

	UFUNCTION(BlueprintCallable, Category = "Rouge|GameState")
	URougeAbilitySystemComponent* GetRougeAbilitySystemComponent() const { return AbilitySystemComponent; }

protected:
	UPROPERTY(Replicated)
	float ServerFPS;

	virtual void Tick(float DeltaSeconds) override;
private:
	UPROPERTY()
	TObjectPtr<URougeExperienceManagerComponent> ExperienceManagerComponent;

	// 被用来向当前的客户端同步游戏状态
	UPROPERTY(VisibleAnywhere, Category = "Rouge|GameState")
	TObjectPtr<URougeAbilitySystemComponent> AbilitySystemComponent;


};
