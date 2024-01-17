// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction_WorldActionBase.h"
#include "Input/RougeMappableConfigPair.h"
#include "GameFeaturesSubsystem.h"
#include "GameFeatureAction_AddInputConfig.generated.h"

class APawn;
struct FComponentRequestHandle;

/**
 * 注册Player的输入Config
 */
UCLASS(meta = (DisplayName = "Add Input Config"))
class ROUGEDEMO_API UGameFeatureAction_AddInputConfig : public UGameFeatureAction_WorldActionBase
{
	GENERATED_BODY()

public:
	//~UObject UGameFeatureAction
	virtual void OnGameFeatureRegistering() override;
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
	virtual void OnGameFeatureUnregistering() override;
	//~End of UGameFeatureAction interface

private:
	/** 用来跟踪Pawn的委托是否被修改 */
	struct FPerContextData
	{
		TArray<TSharedPtr<FComponentRequestHandle>> ExtensionRequestHandles;
		TArray<TWeakObjectPtr<APawn>> PawnsAddedTo;
	};

	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;
	
	//~ Begin UGameFeatureAction_WorldActionBase interface
	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;
	//~ End UGameFeatureAction_WorldActionBase interface

	/** Add all the InputConfigs that are marked to activate automatically to the given pawn */
	void AddInputConfig(APawn* Pawn, FPerContextData& ActiveData);

	/** Remove all the InputConfigs from the given pawn and take them out of the given context data */
	void RemoveInputConfig(APawn* Pawn, FPerContextData& ActiveData);

	void HandlePawnExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);

	UPROPERTY(EditAnywhere)
	TArray<FMappableConfigPair> InputConfigs;
};
