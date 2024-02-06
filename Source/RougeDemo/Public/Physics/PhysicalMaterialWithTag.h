// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "PhysicalMaterialWithTag.generated.h"

/**
 * 
 */
UCLASS()
class ROUGEDEMO_API UPhysicalMaterialWithTag : public UPhysicalMaterial
{
	GENERATED_BODY()

public:
	UPhysicalMaterialWithTag(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// A container of gameplay tags that game code can use to reason about this physical material
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=PhysicalProperties)
	FGameplayTagContainer Tags;
};
