#pragma once

#include "GlobalOptionsStruct.generated.h"

USTRUCT(BlueprintType)
struct FGlobalOptionsStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimGrounded")
	bool bMusic;

};
