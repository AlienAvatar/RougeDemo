// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeleeWeapon.h"
#include "KatanaWeapon.generated.h"

class USceneComponent;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API AKatanaWeapon : public AMeleeWeapon
{
	GENERATED_BODY()

public:
	AKatanaWeapon();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
	TArray<FVector> LastLocationArr;

	UPROPERTY(BlueprintReadWrite)
	FVector Start;

	UPROPERTY(BlueprintReadWrite)
	int ChildCompIndex;
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetLastLocationArrayItem();

	FVector LastLocationKatana;
private:
	FTimerHandle CheckDamageTimerHandle;

	void CheckDamage();

	//virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
