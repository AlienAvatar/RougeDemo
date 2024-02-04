// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "Input/RougeMappableConfigPair.h"
#include "RougeHeroComponent.generated.h"

class URougeAbilitySystemComponent;
class URougeInputConfig;
/**
 * IGameFrameworkInitStateInterface 比如一个obj在初始化前必须要求另一个obj达到某个状态
 * URougeHeroComponent处理摄像机和输入等玩家控制的系统的初始化
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROUGEDEMO_API URougeHeroComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URougeHeroComponent(const FObjectInitializer& ObjectInitializer);
	//返回当前Actor是否包含HeroComponent
	UFUNCTION(BlueprintPure, Category = "Rouge|Hero")
	static URougeHeroComponent* FindHeroComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<URougeHeroComponent>() : nullptr); }
	
	//~ Begin IGameFrameworkInitStateInterface interface
	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	//该函数应该覆盖以执行在特定状态过渡时应发生的特定于对象的更改
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	//它会尽可能告知其他组件将其初始化状态机向前移动
	virtual void CheckDefaultInitialization() override;
	//~ End IGameFrameworkInitStateInterface interface
	
	/**  当输入绑定好时，扩展事件的名称发送给UGameFrameworkComponentManager */
	static const FName NAME_BindInputsNow;

	/** component-implemented 特性名字 */
	static const FName NAME_ActorFeatureName;
	
	void RemoveAdditionalInputConfig(const URougeInputConfig* InputConfig);

	//True 如果被Player控制
	bool IsReadyToBindInputs() const;

	//添加输入设置
	void AddAdditionalInputConfig(const URougeInputConfig* InputConfig);
protected:
	// Called when the game starts
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent);


	//初始化Input设置
	UPROPERTY(EditAnywhere)
	TArray<FMappableConfigPair> DefaultInputConfigs;

	// 当Input已经被绑定应用，则为true
	bool bReadyToBindInputs = false;
	
	void Input_LookMouse(const FInputActionValue& InputActionValue);
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Test();
	
	UPROPERTY()
	TObjectPtr<URougeAbilitySystemComponent> AbilitySystemComponent;
	
	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);
};
