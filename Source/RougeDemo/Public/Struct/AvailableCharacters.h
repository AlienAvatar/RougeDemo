#pragma once

#include "Enum/EActiveAbilities.h"
#include "AvailableCharacters.generated.h"

USTRUCT(BlueprintType)
struct FAvailableCharacters
{
	GENERATED_BODY()

public:
	UPROPERTY()
	EActiveAbilities StartingAbility;

};
