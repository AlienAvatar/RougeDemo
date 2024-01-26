// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "ModularPlayerState.h"
#include "Character/RougePawnData.h"
#include "Teams/RougeTeamAgentInterface.h"
#include "Templates/Casts.h"
#include "RougePlayerState.generated.h"

class URougeAbilitySystemComponent;
class URougePawnData;
class ARougePlayerController;
class ARougeCharacter;
class URougeExperienceDefinition;
/**
 * Rouge玩家状态（ARougePlayerState）拥有用于所有玩家特定技能和行为的技能系统组件
 * 将技能系统组件添加到玩家状态（Player State）类的主要优势是，它有助于将GAS状态逻辑与底层 Pawn 数据分开
 */
UCLASS()
class ROUGEDEMO_API ARougePlayerState : public AModularPlayerState, public IAbilitySystemInterface, public IRougeTeamAgentInterface
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

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	//~IRougeTeamAgentInterface interface
	virtual FOnRougeTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//~End of IRougeTeamAgentInterface interface

	URougeAbilitySystemComponent* GetRougeAbilitySystemComponent() const { return AbilitySystemComponent; }
protected:
	UPROPERTY(ReplicatedUsing = OnRep_PawnData)
	TObjectPtr<const URougePawnData> PawnData;

	UFUNCTION()
	void OnRep_PawnData();

private:
	void OnExperienceLoaded(const URougeExperienceDefinition* CurrentExperience);

	UPROPERTY(VisibleAnywhere, Category = "Rouge|PlayerState")
	TObjectPtr<URougeAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	FOnRougeTeamIndexChangedDelegate OnTeamChangedDelegate;

};
