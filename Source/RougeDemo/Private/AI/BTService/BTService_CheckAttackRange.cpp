// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService/BTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTService_CheckAttackRange::UBTService_CheckAttackRange()
{
	MaxAttackRange = 2000.f;
}

void UBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//检查距离AI和Target
	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if(BlackBoardComp)
	{
		AActor* TargetActor = Cast<AActor>(BlackBoardComp->GetValueAsObject("TargetActor"));
		if (TargetActor)
		{
			AAIController* MyController = OwnerComp.GetAIOwner();
			APawn* AIPawn = MyController->GetPawn();
			if (AIPawn)
			{
				float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());

				bool bWithinRange = FMath::Abs(DistanceTo) < MaxAttackRange;

				//如果true，在范围内
				bool bHasLOS = false;
				if (bWithinRange)
				{
					bHasLOS = MyController->LineOfSightTo(TargetActor);
				}

				BlackBoardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bWithinRange && bHasLOS));
			}
		}
	}
}


