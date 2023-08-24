// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatComponent.h"

#include "AI/BaseAI.h"
#include "Character/PlayerKatanaCharacter.h"
#include "Character/RougeDemoAnimInstance.h"
#include "Character/RougeDemoCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
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

	//Test 扇形
	float AngleDegree = 45.f;
	const FVector Start = RougeDemoCharacter->GetActorLocation();
	const FVector End = RougeDemoCharacter->GetActorForwardVector() * 150.f;
	float CosRadians = FMath::Cos(FMath::Clamp(FMath::DegreesToRadians(AngleDegree),0.f,PI));

	//初始化
	Initialize();
}

void UCombatComponent::Initialize()
{
	SpawnWeapon();
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
			//判断是否可以攻击，如在游泳状态下不能攻击
			if(CheckAttackState())
			{
				//是否在战斗中
				if(!bIsInCombat)
				{
					//后续更改为扇形检测，现检测前方150的位置是否有敌人
					const FVector Start = RougeDemoCharacter->GetActorLocation();
					const FVector End = Start + RougeDemoCharacter->GetActorForwardVector() * 150.f; 
					
					FHitResult HitResult;
					bool bTraceChannel = GetWorld()->LineTraceSingleByChannel(HitResult,Start,End,ECC_Pawn);
					//判断前方是否有敌人，true代表有，false 执行默认
					if(bTraceChannel)
					{
						const ABaseAI* LocalEnemy = Cast<ABaseAI>(HitResult.GetActor());
						//检测敌人是否在玩家前方
						if(LocalEnemy && LocalEnemy->GetDotProductTo(RougeDemoCharacter) > 0.25)
						{
							//当韧值小于0
							if(LocalEnemy->AttributeInfo.ToughnessValue < 0)
							{
								//实施处决效果
								FinisherTimerTrigger();
							}
						}
						
						//不同角色的攻击动画
						APlayerKatanaCharacter* Player = Cast<APlayerKatanaCharacter>(RougeDemoCharacter);
						if(Player)
						{
							Player->PlayAttackAnim();
						}

						//会产生抖动
						FRotator LerpRotation = UKismetMathLibrary::RLerp(
							FRotator(0.f,Player->GetActorRotation().Yaw,0.f),
							FRotator(0.f,Player->GetControlRotation().Yaw,0.f),
							0.1,
							true
						);
						Player->SetActorRotation(LerpRotation);
					}
				}else
				{
					
				}
			}
		}
	}
}

bool UCombatComponent::CheckAttackState()
{
	if(RougeDemoCharacter == nullptr) { return false; }

	bool result = RougeDemoCharacter->GetCharacterMovement()->IsSwimming();
	return !result;
}

void UCombatComponent::SpawnWeapon()
{
	if(RougeDemoCharacter == nullptr) return;
	if(CurrentWeaponClass)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = GetOwner();
		const FTransform UnArmSocketTransform = RougeDemoCharacter->GetMesh()->GetSocketTransform(FName("R_Weapon_UnArmKatana_Socket"));
		CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(CurrentWeaponClass,UnArmSocketTransform,SpawnParameters);
		CurrentWeapon->AttachToComponent(RougeDemoCharacter->GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale,FName("R_Weapon_UnArmKatana_Socket"));
	}
}


void UCombatComponent::EquipWeapon()
{
	if(RougeDemoCharacter == nullptr)
	{
		return;
	}
	
	if(CurrentWeapon)
	{
		CurrentWeapon->AttachToComponent(RougeDemoCharacter->GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale,FName("R_Weapon_Katana_Socket"));
		CurrentWeapon->GetWeaponMesh()->SetVisibility(true);

		//设置static为鞘
		const APlayerKatanaCharacter* PlayerKatanaCharacter = Cast<APlayerKatanaCharacter>(RougeDemoCharacter);
		if(PlayerKatanaCharacter)
		{
			UStaticMesh* KatanaAndScabbardMeshRef = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/RougeDemo/RES/Mesh/Weapon/Katana/SM_Scabbard01.SM_Scabbard01"));
			if(KatanaAndScabbardMeshRef)
			{
				PlayerKatanaCharacter->GetKatanaMeshComp()->SetStaticMesh(KatanaAndScabbardMeshRef);
			}
		}
	}
}

void UCombatComponent::UnArmWeapon()
{
	if(RougeDemoCharacter == nullptr) return;
	if(CurrentWeapon)
	{
		
		CurrentWeapon->AttachToComponent(RougeDemoCharacter->GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale,FName("R_Weapon_KatanaAndScabbard_Socket"));
		CurrentWeapon->GetWeaponMesh()->SetVisibility(false);

		//重新设置static为刀鞘
		const APlayerKatanaCharacter* PlayerKatanaCharacter = Cast<APlayerKatanaCharacter>(RougeDemoCharacter);
		UStaticMesh* KatanaAndScabbardMeshRef = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/RougeDemo/RES/Mesh/Weapon/Katana/SM_KatanaAndScabbard.SM_KatanaAndScabbard"));
		if(PlayerKatanaCharacter)
		{
			if(KatanaAndScabbardMeshRef)
			{
				PlayerKatanaCharacter->GetKatanaMeshComp()->SetStaticMesh(KatanaAndScabbardMeshRef);
			}
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

			if(ExecutionEnemyRef->bCanBeExecuted && IsIdleAndFalling())
			{
				OnFinisherStart();
			}else
			{
				//普通攻击
				
			}
		}
	}
}

void UCombatComponent::OnFinisherStart()
{
	if(ExecutionEnemyRef)
	{
		//停止敌人角色移动
		ExecutionEnemyRef->GetCharacterMovement()->DisableMovement();
		//停止敌人角色动画
		ExecutionEnemyRef->StopAnimMontage();
		//停止玩家所有输入
		RougeDemoCharacter->SetDisableInput(true);
		//关闭目标的锁定UI
		ExecutionEnemyRef->ToggleMarket(false);

		URougeDemoAnimInstance* AnimInstance = Cast<URougeDemoAnimInstance>(RougeDemoCharacter->GetMesh()->GetAnimInstance());
		if(AnimInstance)
		{
			//播放处决动画
			if(FinisherMontage)
			{
				AnimInstance->Montage_Play(FinisherMontage);
			}
		}
		
	}
}

bool UCombatComponent::IsIdleAndFalling()
{
	if(RougeDemoCharacter == nullptr) return false;

	return !RougeDemoCharacter->GetCharacterMovement()->IsFalling();
}

// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

