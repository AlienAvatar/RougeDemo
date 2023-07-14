#pragma once

#include "VelocityBlend.generated.h"

USTRUCT(BlueprintType)
struct FVelocityBlend
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimGrounded")
	float F;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimGrounded")
	float B;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimGrounded")
	float L;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimGrounded")
	float R;
};
