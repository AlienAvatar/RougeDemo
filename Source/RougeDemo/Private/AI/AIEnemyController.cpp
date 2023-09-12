// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIEnemyController.h"

#include "AI/BaseAI.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"

AAIEnemyController::AAIEnemyController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	
}

void AAIEnemyController::BeginPlay()
{
	Super::BeginPlay();

	//绑定函数
	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this,&AAIEnemyController::OnPerceptionUpdatedHandle);
}

void AAIEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(InPawn == nullptr) return;

	ABaseAI* BaseAI = Cast<ABaseAI>(InPawn);
	if(BaseAI)
	{
		BlackboardComponent->InitializeBlackboard(*EnemyBlackBoard);
		if(EnemyBlackBoard)
		{
			if(EnemyBehaviorTree)
			{
				RunBehaviorTree(EnemyBehaviorTree);
			}
		}
		float AttackRadius = BaseAI->GetIdealRange();
		//给黑板设定初始值
		BlackboardComponent->SetValueAsFloat(FName("AttackRadius"),AttackRadius);
	}
}

void AAIEnemyController::OnPerceptionUpdatedHandle(const TArray<AActor*>& UpdatedActors)
{
	FAIStimulus AIStimulus;
	for(auto UpdateActor : UpdatedActors)
	{
		//是否是视觉感知到Actor
		bool bSightSensed = CanSenseActor(UpdateActor,EAISense::EAIS_Sight,AIStimulus);
		if(bSightSensed)
		{
			HandleSightSensed(UpdateActor);
		}
		
		//是否是听觉感知到Actor
		bool bHearSensed = CanSenseActor(UpdateActor,EAISense::EAIS_Hearing,AIStimulus);
		if(bHearSensed)
		{
			
			HandleHearingSensed(AIStimulus.ReceiverLocation);
		}
		
		//是否是伤害感知到Actor
		bool bDamageSensed = CanSenseActor(UpdateActor,EAISense::EAIS_Damage,AIStimulus);
		if(bDamageSensed)
		{
			HandleDamageSensed(UpdateActor);
		}
	}
}

bool AAIEnemyController::CanSenseActor(AActor* Actor, EAISense AISense, FAIStimulus& AIStimulus)
{
	
	TSubclassOf<UAISense> ConfigClass;
	switch (AISense)
	{
	case EAISense::EAIS_Sight:
		ConfigClass = UAISense_Sight::StaticClass();
		break;
	case EAISense::EAIS_Hearing:
		ConfigClass = UAISense_Hearing::StaticClass();
		break;
	case EAISense::EAIS_Damage:
		ConfigClass = UAISense_Damage::StaticClass();
		break;
	case EAISense::EAI_None:
		ConfigClass = nullptr;
		break;
	default:
		UE_LOG(LogTemp,Error,TEXT("No setting AISense"))
		return false;
	}
	
	//获取当前感知
	FActorPerceptionBlueprintInfo ActorPerceptionBlueprintInfo;
	AIPerceptionComponent->GetActorsPerception(Actor,ActorPerceptionBlueprintInfo);

	//遍历设置的感知元件
	TArray<FAIStimulus> AIStimulusArr = ActorPerceptionBlueprintInfo.LastSensedStimuli;
	for(auto i : AIStimulusArr)
	{
		//获取当前的感知元件的Class
		TSubclassOf<UAISense> AISenseClass = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(),i);
		if(ConfigClass == AISenseClass)
		{
			//返回当前感知元件的信息
			AIStimulus = i;
			return i.WasSuccessfullySensed();
		}
	}
	return false;
}

void AAIEnemyController::HandleSightSensed(AActor* Actor)
{
	ABaseAI* BaseAI = Cast<ABaseAI>(GetPawn());
	if(BaseAI)
	{
		BaseAI->State = EState::ES_Attacking;
	}
	AttackTarget = Actor;
	GetBlackboardComponent()->SetValueAsObject(FName("TargetActor"),Actor);
}

void AAIEnemyController::HandleHearingSensed(FVector Location)
{
}

void AAIEnemyController::HandleDamageSensed(AActor* Actor)
{
	ABaseAI* BaseAI = Cast<ABaseAI>(GetPawn());
	if(BaseAI)
	{
		BaseAI->State = EState::ES_Attacking;
	}
	AttackTarget = Actor;
	GetBlackboardComponent()->SetValueAsObject(FName("TargetActor"),Actor);
}
