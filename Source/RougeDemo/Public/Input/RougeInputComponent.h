// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "RougeInputComponent.generated.h"


class UEnhancedInputLocalPlayerSubsystem;
class URougeInputConfig;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROUGEDEMO_API URougeInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URougeInputComponent(const FObjectInitializer& ObjectInitializer);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void AddInputMappings(const URougeInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const;
};
