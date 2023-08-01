#pragma once
#include "FinisherCamera.h"
#include "FinisherDamage.h"
#include "Enum/EFinisherCategories.h"
#include "Enum/EFinisherMoveToSettings.h"
#include "Enum/EFinisherCameraType.h"

#include "FinisherBase.generated.h"

USTRUCT(BlueprintType)
struct FFinisherBase : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* MontageAttacker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* MontageTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	EFinisherCategories Category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	EFinisherMoveToSettings MoveToSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FFinisherDamage DamageSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	EFinisherCameraType CameraType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FFinisherCamera CinematicCameraSettings;
};
