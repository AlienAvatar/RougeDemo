// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "Enum/EAISense.h"
#include "AIEnemyController.generated.h"

class UBlackboardData;
class UBlackboardComponent;
class UBehaviorTreeComponent;
class UAIPerceptionComponent;
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
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UBlackboardData* EnemyBlackBoard;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UBehaviorTree* EnemyBehaviorTree;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAIPerceptionComponent* AIPerceptionComponent;
private:

	bool bIsRanged = false;

	UFUNCTION()
	void OnPerceptionUpdatedHandle(const TArray<AActor*>& UpdatedActors);

	bool CanSenseActor(AActor* Actor, EAISense AISense, FAIStimulus& AIStimulus);

	void HandleSightSensed(AActor* Actor);

	void HandleHearingSensed(FVector Location);

	void HandleDamageSensed(AActor* Actor);

	
};
