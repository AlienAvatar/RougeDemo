// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatComponent.h"

#include "AI/BaseAI.h"
#include "Character/RougeDemoCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/Weapon.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	RougeDemoCharacter = Cast<ARougeDemoCharacter>(GetOwner());

	float AngleDegree = 45.f;
	const FVector Start = RougeDemoCharacter->GetActorLocation();
	const FVector End = RougeDemoCharacter->GetActorForwardVector() * 150.f;
	float CosRadians = FMath::Cos(FMath::Clamp(FMath::DegreesToRadians(AngleDegree),0.f,PI));

	//初始化
	Initialize();
}

void UCombatComponent::Initialize()
{
	EquipWeapon();
}

void UCombatComponent::Attack()
{
	//是否在空中或者飞行状态
	//若是，进行空中攻击
	if(RougeDemoCharacter)
	{
		if(RougeDemoCharacter->GetCharacterMovement()->IsFlying())
		{
			//To Do 空中攻击
		}else
		{
			//地面攻击
			//判断是否可以攻击，如在某些状态下不能攻击
			if(!AttackBoolDisabled())
			{
				if(bIsInCombat)
				{
					
				}else
				{
					
				}
			}
		}
	}
	
	
	

	//判断是否正在攻击，是否处于攻击间隔中

	//检查武器Slot

	//判断攻击类型 物理，魔法

	
}

bool UCombatComponent::AttackBoolDisabled()
{
	
	return false;
}


void UCombatComponent::EquipWeapon()
{
	if(RougeDemoCharacter == nullptr) return;
	if(CurrentWeaponClass)
	{
		const USkeletalMeshSocket* HandSocket = RougeDemoCharacter->GetMesh()->GetSocketByName(FName("R_Weapon_Socket"));
		const FTransform RightHandTransform = HandSocket->GetSocketTransform(RougeDemoCharacter->GetMesh());
		CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(CurrentWeaponClass,RightHandTransform);
		if(HandSocket)
		{
			HandSocket->AttachActor(CurrentWeapon,RougeDemoCharacter->GetMesh());
		}
	}
}

void UCombatComponent::FinisherTimerTrigger()
{
	if(CombatType == ECombatType::ECT_Melee)
	{
		FHitResult HitResult;
		//发出射线进行检测 后续需改为扇形检测
		const FVector Start = RougeDemoCharacter->GetActorLocation();
		const FVector End = RougeDemoCharacter->GetActorForwardVector();
		GetWorld()->LineTraceSingleByChannel(HitResult,Start,End,ECC_Pawn);
		if(HitResult.bBlockingHit)
		{
			//判断Enmey的状态，是Idle而不是Falling
			ExecutionEnemyRef = Cast<ABaseAI>(HitResult.GetActor());

			
		}
	}
}

// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

