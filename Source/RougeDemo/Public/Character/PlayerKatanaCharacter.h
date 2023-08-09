// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/RougeDemoCharacter.h"
#include "PlayerKatanaCharacter.generated.h"

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
	void TestAction();

	FTimerHandle MontageFinishTimer;

	void MontageFinishTimerCallBack();
protected:
	UPROPERTY(EditAnywhere,Category=Combat)
	UAnimMontage* EquipAnimMontageRoot;

	UPROPERTY(EditAnywhere,Category=Combat)
	UAnimMontage* EquipAnimMontageInPlace;
	
	UPROPERTY(EditAnywhere,Category=Combat)
	UAnimMontage* UnArmAnimMontagesRoot;

	UPROPERTY(EditAnywhere,Category=Combat)
	UAnimMontage* UnArmAnimMontageInPlace;

	UPROPERTY(EditAnywhere,Category=Combat)
	UStaticMeshComponent* KatanaMeshComp;
	

	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


};
