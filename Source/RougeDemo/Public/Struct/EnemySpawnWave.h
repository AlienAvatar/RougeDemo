#pragma once
#include "EnemySpawnType.h"
#include "Engine/DataTable.h"

#include "EnemySpawnWave.generated.h"

class ASoul;
class ABaseAI;

USTRUCT(BlueprintType)
struct FEnemySpawnWave : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEnemySpawnType> SpawnArr;
	
};
