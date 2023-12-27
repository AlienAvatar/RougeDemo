#pragma once
#include "Engine/DataTable.h"
#include "Enum/EActiveAbilities.h"
#include "AvailableCharacter.generated.h"

USTRUCT(BlueprintType)
struct FAvailableCharacter : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EActiveAbilities ActiveAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* CharacterSK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDefaultLocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText CharacterName;
};
