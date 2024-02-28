// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkComponent.h"
#include "RougeManaComponent.generated.h"

class URougeAbilitySystemComponent;
class URougeHealthSet;
struct FGameplayEffectSpec;
struct FOnAttributeChangeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FRougeMana_AttributeChanged, URougeManaComponent*, ManaComponent,
                                              float, OldValue, float, NewValue, AActor*, Instigator);

//没有使用
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROUGEDEMO_API URougeManaComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URougeManaComponent(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable)
	FRougeMana_AttributeChanged OnManaChanged;

	UPROPERTY(BlueprintAssignable)
	FRougeMana_AttributeChanged OnMaxManaChanged;

	// Initialize the component using an ability system component.
	UFUNCTION(BlueprintCallable, Category = "Rouge|Mana")
	void InitializeWithAbilitySystem(URougeAbilitySystemComponent* InASC);

	UFUNCTION(BlueprintCallable, Category = "Rouge|Mana")
	float GetMana() const;

	UFUNCTION(BlueprintCallable, Category = "Rouge|Mana")
	float GetMaxMana() const;

	UFUNCTION(BlueprintCallable, Category = "Rouge|Mana")
	float GetManaNormalized() const;

	UFUNCTION(BlueprintPure, Category = "Rouge|Mana")
	static URougeManaComponent* FindManaComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<URougeManaComponent>() : nullptr); }
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<const URougeHealthSet> RougeSet;

	UPROPERTY()
	TObjectPtr<URougeAbilitySystemComponent> AbilitySystemComponent;

	virtual void HandleManaChanged(const FOnAttributeChangeData& ChangeData);
	virtual void HandleMaxManaChanged(const FOnAttributeChangeData& ChangeData);

	void ClearGameplayTags();
public:
	
};
