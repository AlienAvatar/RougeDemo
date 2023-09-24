// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/RougeDemoCharacter.h"
#include "PlayerKatanaCharacter.generated.h"

class ARougeDemoPlayerController;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API APlayerKatanaCharacter : public ARougeDemoCharacter
{
	GENERATED_BODY()

public:
	APlayerKatanaCharacter();
	
private:
	void EquipOrUnarmAction();

	FTimerHandle EquippedMontageFinishTimer;

	void EquippedMontageFinishTimerCallBack();

	FTimerHandle UnarmMontageFinishTimer;

	void UnarmMontageFinishTimerCallBack();

	void TestAction();
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere,Category=Combat)
	UAnimMontage* EquipAnimMontageRoot;

	UPROPERTY(EditAnywhere,Category=Combat)
	UAnimMontage* EquipAnimMontageInPlace;
	
	UPROPERTY(EditAnywhere,Category=Combat)
	UAnimMontage* UnArmAnimMontagesRoot;

	UPROPERTY(EditAnywhere,Category=Combat)
	UAnimMontage* UnArmAnimMontageInPlace;

	UPROPERTY(EditAnywhere,Category=Combat)
	UAnimMontage* AttackMontageRoot;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void AttackAction() override;

	UPROPERTY(EditAnywhere,Category=Combat)
	UStaticMeshComponent* KatanaMeshComp;

	UPROPERTY(EditDefaultsOnly)
	TArray<FName> AttackSectionArr;

	UPROPERTY(BlueprintReadWrite)
	int32 AttackIndex = 0;

	void PlayUnarmAnim();

	void EndMontageCallback(UAnimMontage* Montage, bool bInterrupted);
public:
	FORCEINLINE UStaticMeshComponent* GetKatanaMeshComp() const { return KatanaMeshComp; }

	void PlayAttackAnim();

	void PlayEquippedAnim();

	//逻辑控制装备是否装备
	bool bEquipped = false;

	//用来判断是攻击命令还是装备命令
	UPROPERTY(BlueprintReadWrite)
	bool bEquippedToAttack = false;

	UFUNCTION(BlueprintCallable)
	void ResetOverlayState();
};
