// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ModularPlayerState.h"
#include "Character/RougePawnData.h"
#include "Templates/Casts.h"
#include "RougePlayerState.generated.h"

class URougePawnData;
class ARougePlayerController;
class ARougeCharacter;
class URougeExperienceDefinition;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API ARougePlayerState : public AModularPlayerState
{
	GENERATED_BODY()

private:
	UPROPERTY()
	ARougeCharacter* RougeDemoCharacter;

	UPROPERTY()
	ARougePlayerController* RougeDemoPlayerController;
public:
	
	virtual void OnRep_Score() override;
	
	void AddToScore(float ScoreAmount);

	const URougePawnData* GetPawnData() const { return Cast<URougePawnData>(PawnData); }

	void SetPawnData(const URougePawnData* InPawnData);
protected:
	UPROPERTY(ReplicatedUsing = OnRep_PawnData)
	TObjectPtr<const URougePawnData> PawnData;

	UFUNCTION()
	void OnRep_PawnData();

private:
	void OnExperienceLoaded(const URougeExperienceDefinition* CurrentExperience);

};
