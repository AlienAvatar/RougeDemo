// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIEnemyController.h"

#include "AI/BaseAI.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AAIEnemyController::AAIEnemyController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
}

void AAIEnemyController::BeginPlay()
{
	Super::BeginPlay();

	if(EnemyBlackBoard)
	{
		if(EnemyBehaviorTree)
		{
			RunBehaviorTree(EnemyBehaviorTree);
		}
	}
}

void AAIEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(InPawn == nullptr) return;

	ABaseAI* BaseAI = Cast<ABaseAI>(InPawn);
	if(BaseAI)
	{
		BlackboardComponent->InitializeBlackboard(*EnemyBlackBoard);
	}
}
