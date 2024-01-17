// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "RougePawnExtensionComponent.generated.h"


class URougeAbilitySystemComponent;
class URougePawnData;
/**
 * 挂载到RougeCharacter
 * IGameFrameworkInitStateInterface 用于解决当obj初始化时的状态时序关系问题，比如一个obj在初始化前必须要求另一个obj达到某个状态
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROUGEDEMO_API URougePawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	static const FName NAME_ActorFeatureName;
	// Sets default values for this component's properties
	URougePawnExtensionComponent(const FObjectInitializer& ObjectInitializer);

	// 注册并调用委托，如果我们的Pawn已经注册了AbilitySystemComponent，那么就注册OnAbilitySystemInitialized委托
	void OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate);

	//注销委托
	void OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate Delegate);

	UFUNCTION(BlueprintPure, Category = "Lyra|Pawn")
	static URougePawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<URougePawnExtensionComponent>() : nullptr); }

	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// 当我们的 pawn 成为 ability system 的 avatar actor时，委托触发
	// 多播委托对象
	FSimpleMulticastDelegate OnAbilitySystemInitialized;

	//当我们的Pawn移除 ability system 的 avatar actor 时，委托触发
	FSimpleMulticastDelegate OnAbilitySystemUninitialized;
	
	// 缓存AbilitySystemComponent
	UPROPERTY()
	TObjectPtr<URougeAbilitySystemComponent> AbilitySystemComponent;

	/** Pawn data 用来创建 pawn. 从Spwan函数或者实例中指定 */
	UPROPERTY(EditInstanceOnly, Category = "Lyra|Pawn")
	TObjectPtr<const URougePawnData> PawnData;
	
	// UFUNCTION()
	// void OnRep_PawnData();
	//
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	//获取当前的AbilitySystemComponent，可能来自不同的Actor
	UFUNCTION(BlueprintPure, Category = "Rouge|Pawn")
	URougeAbilitySystemComponent* GetRougeAbilitySystemComponent() const { return AbilitySystemComponent; }

	void SetupPlayerInputComponent();

	//GameFreak Interface Start
	// 重写，尝试读取默认的初始化路径
	virtual void CheckDefaultInitialization() override;
};
