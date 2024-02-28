// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RougeCharacter.h"
#include "ManaRougeCharacter.generated.h"

class URougeManaComponent;

UCLASS()
class ROUGEDEMO_API AManaRougeCharacter : public ARougeCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AManaRougeCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URougeManaComponent* ManaComponent;
	
protected:
	virtual void OnAbilitySystemInitialized() override;
};
