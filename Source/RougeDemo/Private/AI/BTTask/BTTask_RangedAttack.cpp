// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_RangedAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/BaseAI.h"

UBTTask_RangedAttack::UBTTask_RangedAttack()
{
	MaxBulletSpread = 2.0f;
}

EBTNodeResult::Type UBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if(MyController)
	{
		//检查是否属于Character
		ABaseAI* MyPawn = Cast<ABaseAI>(MyController->GetPawn());
		if (MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		//检查是否存活
		if (MyPawn && !MyPawn->IsAlive())
		{
			return EBTNodeResult::Failed;
		}
		
		//是否有Target
		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		MyPawn->PlayRangeAttackAnim();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
