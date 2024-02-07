// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "RougeAttributeSet.generated.h"

class URougeAbilitySystemComponent;
/**
 * URougeAttributeSet
 */
UCLASS()
class ROUGEDEMO_API URougeAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	URougeAttributeSet();

	UWorld* GetWorld() const override;

	URougeAbilitySystemComponent* GetRougeAbilitySystemComponent() const;
};
