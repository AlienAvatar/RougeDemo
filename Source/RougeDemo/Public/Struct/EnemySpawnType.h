#pragma once
#include "Engine/DataTable.h"

#include "EnemySpawnType.generated.h"

class ASoul;
class ABaseAI;

USTRUCT(BlueprintType)
struct FEnemySpawnType : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABaseAI> Enemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AllowedWave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ASoul> Soul;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bElite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bScaleHPtoLevel;
	
};
