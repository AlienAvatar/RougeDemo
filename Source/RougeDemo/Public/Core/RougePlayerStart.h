// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "RougePlayerStart.generated.h"

enum class ERougePlayerStartLocationOccupancy
{
	Empty,
	Partial,
	Full
};

/**
 * 游戏初始位置
 */
UCLASS()
class ROUGEDEMO_API ARougePlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	ARougePlayerStart(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	ERougePlayerStartLocationOccupancy GetLocationOccupancy(AController* const ControllerPawnToFit) const;
	bool TryClaim(AController* OccupyingController);

	//这个玩家是否已经开始被PlayerController认领了?
	bool IsClaimed() const;

protected:
	//Controller用来认领PlayerStart
	UPROPERTY(Transient)
	TObjectPtr<AController> ClaimingController = nullptr;

	/** Handle to track expiration recurring timer */
	FTimerHandle ExpirationTimerHandle;

	/** 检查PlayerStart是否被认领 */
	void CheckUnclaimed();

	//检查间隔
	UPROPERTY(EditDefaultsOnly, Category = "Player Start Claiming")
	float ExpirationCheckInterval = 1.f;

};
