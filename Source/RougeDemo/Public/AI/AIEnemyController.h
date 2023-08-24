// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIEnemyController.generated.h"

class UBlackboardData;
class UBlackboardComponent;
class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API AAIEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	AAIEnemyController();
	
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const { return BlackboardComponent; } 
protected:
	UPROPERTY(EditAnywhere)
	UBlackboardComponent* BlackboardComponent;

	UPROPERTY(EditAnywhere)
	UBehaviorTreeComponent* BehaviorTreeComponent;
	
	UPROPERTY(EditAnywhere)
	UBlackboardData* EnemyBlackBoard;
	
	UPROPERTY(EditAnywhere)
	UBehaviorTree* EnemyBehaviorTree;

	
private:

	bool bIsRanged = false;
};
