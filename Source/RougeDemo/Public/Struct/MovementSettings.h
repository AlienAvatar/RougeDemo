#pragma once

#include"Engine/Datatable.h"
#include "MovementSettings.generated.h"

class UCurveVector;
class UCurveFloat;

USTRUCT(BlueprintType)
struct FMovementSettings : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterMovement")
	float WalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterMovement")
	float RunSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterMovement")
	float SprintSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterMovement")
	UCurveVector* MovementCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterMovement")
	UCurveFloat* RotationRateCurve;
};