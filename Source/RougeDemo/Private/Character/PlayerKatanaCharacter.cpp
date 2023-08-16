// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerKatanaCharacter.h"

#include "Character/RougeDemoAnimInstance.h"
#include "Components/CombatComponent.h"

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

void APlayerKatanaCharacter::TestAction()
{
	if(CombatComp == nullptr || RougeDemoAnimInstance == nullptr){ return;}
	
	float Duration = 0.2f;
	if(OverlayState == EOverlayState::EOS_Katana)
	{
		//检查是否可以播放蒙太奇动画
		if(bCanPlayMontage)
		{
			OverlayState = EOverlayState::EOS_Default;
			bCanPlayMontage = false;

			//速度小于10，用root收刀，>10,用Inplace收刀
			if(Speed < 10.f)
			{
				if(UnArmAnimMontagesRoot)
				{
					Duration = RougeDemoAnimInstance->Montage_Play(UnArmAnimMontagesRoot,1.f);
				}
			}else
			{
				if(UnArmAnimMontageInPlace)
				{
					Duration = RougeDemoAnimInstance->Montage_Play(UnArmAnimMontageInPlace,1.5f);
					Duration /= 1.5f;
				}
			}

			GetWorld()->GetTimerManager().SetTimer(
				MontageFinishTimer,
				this,
				&APlayerKatanaCharacter::MontageFinishTimerCallBack,
				Duration
			);
		}
	}else
	{
		if(bCanPlayMontage)
		{
			OverlayState = EOverlayState::EOS_Katana;
			bCanPlayMontage = false;

			if(Speed < 10.f)
			{
				if(EquipAnimMontageRoot)
				{
					Duration = RougeDemoAnimInstance->Montage_Play(EquipAnimMontageRoot,1.f);
				}
			}else
			{
				if(EquipAnimMontageInPlace)
				{
					Duration = RougeDemoAnimInstance->Montage_Play(EquipAnimMontageInPlace,1.5f);
				}
			}
			

			GetWorld()->GetTimerManager().SetTimer(
				MontageFinishTimer,
				this,
				&APlayerKatanaCharacter::MontageFinishTimerCallBack,
				Duration
			);
		}
	}
}

void APlayerKatanaCharacter::MontageFinishTimerCallBack()
{
	bCanPlayMontage = true;
}

void APlayerKatanaCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(KatanaMeshComp)
	{
		KatanaMeshComp->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale,FName("R_Weapon_KatanaAndScabbard_Socket"));	
	}
}

void APlayerKatanaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Test",IE_Pressed,this,&APlayerKatanaCharacter::TestAction);
	PlayerInputComponent->BindAction("Attack",IE_Pressed,this,&APlayerKatanaCharacter::AttackAction);
}

void APlayerKatanaCharacter::AttackAction()
{
	/*if(OverlayState != EOverlayState::EOS_Katana)
	{
		TestAction();
	}*/
	if(!bCanPlayMontage) { return; }

	if(CombatComp)
	{
		CombatComp->Attack();
	}
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
