#pragma once

#include"Engine/Datatable.h"
#include "MovementSettings.h"
#include "MovementSettingsStance.generated.h"

USTRUCT(BlueprintType)
struct FMovementSettingsStance :  public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMovementSettings Standing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMovementSettings Crouching;
};
