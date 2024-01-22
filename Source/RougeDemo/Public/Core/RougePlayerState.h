// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ModularPlayerState.h"
#include "Character/RougePawnData.h"
#include "Templates/Casts.h"
#include "RougePlayerState.generated.h"

class URougeAbilitySystemComponent;
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

public:
	ARougePlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
private:
	UPROPERTY()
	ARougeCharacter* RougeDemoCharacter;

	UPROPERTY()
	ARougePlayerController* RougeDemoPlayerController;
public:
	//~APlayerState interface
	virtual void Reset() override;
	virtual void ClientInitialize(AController* C) override;
	virtual void CopyProperties(APlayerState* PlayerState) override;
	virtual void OnDeactivated() override;
	virtual void OnReactivated() override;
	//~End of APlayerState interface

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	//~End of AActor interface

	static const FName NAME_RougeAbilityReady;
	
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

	UPROPERTY(VisibleAnywhere, Category = "Rouge|PlayerState")
	TObjectPtr<URougeAbilitySystemComponent> AbilitySystemComponent;
};
