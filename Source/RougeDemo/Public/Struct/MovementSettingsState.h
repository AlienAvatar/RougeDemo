#pragma once

#include"Engine/Datatable.h"
#include "MovementSettingsStance.h"
#include "MovementSettingsState.generated.h"

USTRUCT(BlueprintType)
struct FMovementSettingsState : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMovementSettingsStance VelocityDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMovementSettingsStance LookingDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMovementSettingsStance Aiming;
};
