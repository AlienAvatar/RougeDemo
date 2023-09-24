// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerKatanaCharacter.h"

#include "Character/RougeDemoAnimInstance.h"
#include "Components/CombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

APlayerKatanaCharacter::APlayerKatanaCharacter()
{
	FStringAssetReference SkletalPath(TEXT("/Script/Engine.SkeletalMesh'/Game/FemaleMilitaryOfficer/Characters/Meshes/SK_Tubaki.SK_Tubaki'"));
	USkeletalMesh* SkeletalMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, *SkletalPath.ToString()));
	if (SkeletalMesh)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMesh);
	}

	FStringAssetReference BlueprintPath(TEXT("/Script/Engine.AnimBlueprint'/Game/RougeDemo/SRC/Blueprints/Character/TubaKi/Anim/ABP_Tsubaki.ABP_Tsubaki'"));
	UAnimBlueprint* BlueprintRef = Cast<UAnimBlueprint>(StaticLoadObject(UAnimBlueprint::StaticClass(), nullptr, *BlueprintPath.ToString()));
	if(BlueprintRef)
	{
		GetMesh()->SetAnimClass(BlueprintRef->GetAnimBlueprintGeneratedClass());
	}

	KatanaMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KatanaMeshComp"));
	KatanaMeshComp->SetupAttachment(GetMesh());
	UStaticMesh* KatanaAndScabbardMeshRef = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/RougeDemo/RES/Mesh/Weapon/Katana/SM_KatanaAndScabbard.SM_KatanaAndScabbard"));
	if(KatanaAndScabbardMeshRef)
	{
		KatanaMeshComp->SetStaticMesh(KatanaAndScabbardMeshRef);
	}
	//关闭刀鞘的碰撞
	KatanaMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}



void APlayerKatanaCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(KatanaMeshComp)
	{
		KatanaMeshComp->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale,FName("R_Weapon_KatanaAndScabbard_Socket"));	
	}
}

void APlayerKatanaCharacter::EquipOrUnarmAction()
{
	if(CombatComp == nullptr || RougeDemoAnimInstance == nullptr){ return; }
	
	//检查是否正在播放蒙太奇动画
	if(!RougeDemoAnimInstance->IsAnyMontagePlaying())
	{
		if(!bEquipped)
		{
			//更改动画状态
			OverlayState = EOverlayState::EOS_Katana;

			//播放拔刀动画
			PlayEquippedAnim();
		}else
		{
			//更改动画状态
			OverlayState = EOverlayState::EOS_Default;
			//播放收刀动画
			PlayUnarmAnim();
		}
	}
}

void APlayerKatanaCharacter::EquippedMontageFinishTimerCallBack()
{
	bEquipped = true;
	//bCanPlayMontage = true;
}



void APlayerKatanaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("EquipOrUnarm",IE_Pressed,this,&APlayerKatanaCharacter::EquipOrUnarmAction);
	PlayerInputComponent->BindAction("Test",IE_Pressed,this,&APlayerKatanaCharacter::TestAction);
	PlayerInputComponent->BindAction("Attack",IE_Pressed,this,&APlayerKatanaCharacter::AttackAction);
}

void APlayerKatanaCharacter::AttackAction()
{
	if(!bCanPlayMontage) { return; }

	if(CombatComp)
	{
		if(!bEquipped)
		{
			//更改动画状态
			OverlayState = EOverlayState::EOS_Katana;
			//如果武器在背上，先拔出武器
			PlayEquippedAnim();
			//说明是从攻击命令下拔刀的
			bEquippedToAttack = true;
		}
		
		if(bEquipped)
		{
			CombatComp->Attack();
		}
	}
}

void APlayerKatanaCharacter::PlayUnarmAnim()
{
	if(CombatComp == nullptr || RougeDemoAnimInstance == nullptr){ return; }
	
	float LocalDuration = 0.2f;
	//速度小于10，用root收刀，>10,用Inplace收刀
	if(Speed < 10.f)
	{
		if(UnArmAnimMontagesRoot)
		{
			LocalDuration = RougeDemoAnimInstance->Montage_Play(UnArmAnimMontagesRoot,1.5f);
		}
	}else
	{
		if(UnArmAnimMontageInPlace)
		{
			LocalDuration = RougeDemoAnimInstance->Montage_Play(UnArmAnimMontageInPlace,1.5f);
		}
	}
	bEquipped = true;
	//RougeDemoAnimInstance->OnMontageEnded.AddDynamic(this,&APlayerKatanaCharacter::EndMontageCallback);
	
	/*GetWorld()->GetTimerManager().SetTimer(
		UnarmMontageFinishTimer,
		this,
		&APlayerKatanaCharacter::UnarmMontageFinishTimerCallBack,
		LocalDuration
	);*/
}

void APlayerKatanaCharacter::EndMontageCallback(UAnimMontage* Montage, bool bInterrupted)
{
	Montage->PostLoad();
}

void APlayerKatanaCharacter::UnarmMontageFinishTimerCallBack()
{
	bEquipped = false;
	bEquippedToAttack = false;
}

void APlayerKatanaCharacter::TestAction()
{
}

void APlayerKatanaCharacter::PlayAttackAnim()
{
	if(RougeDemoAnimInstance && AttackMontageRoot && AttackIndex < 4)
	{
		bCanPlayMontage = false;
		const FName MontageSection = AttackSectionArr[AttackIndex];
		RougeDemoAnimInstance->Montage_Play(AttackMontageRoot);
		RougeDemoAnimInstance->Montage_JumpToSection(MontageSection,AttackMontageRoot);
		AttackIndex++;
	}
}

void APlayerKatanaCharacter::PlayEquippedAnim()
{
	if(CombatComp == nullptr || RougeDemoAnimInstance == nullptr){ return; }
	
	float LocalDuration = 0.2f;
	//检查是否有正在播放蒙太奇动画
	if(RougeDemoAnimInstance->IsAnyMontagePlaying())
	{
		return;
	}else
	{
		//速度小于10，用Root播放，反之用Inplace
		if(Speed < 10.f)
		{
			if(EquipAnimMontageRoot)
			{
				LocalDuration = RougeDemoAnimInstance->Montage_Play(EquipAnimMontageRoot,1.5f);
			}
		}else
		{
			if(EquipAnimMontageInPlace)
			{
				LocalDuration = RougeDemoAnimInstance->Montage_Play(EquipAnimMontageInPlace,1.5f);
			}
		}

		GetWorld()->GetTimerManager().SetTimer(
			EquippedMontageFinishTimer,
			this,
			&APlayerKatanaCharacter::EquippedMontageFinishTimerCallBack,
			LocalDuration
		);
	}
}

void APlayerKatanaCharacter::ResetOverlayState()
{
	//更改动画状态
	OverlayState = EOverlayState::EOS_Default;
}
