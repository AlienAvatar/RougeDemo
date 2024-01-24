// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "RougePlayerSpawningManagerComponent.generated.h"

class ARougePlayerStart;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROUGEDEMO_API URougePlayerSpawningManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URougePlayerSpawningManagerComponent(const FObjectInitializer& ObjectInitializer);

	/** UActorComponent */
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	/** ~UActorComponent */
	void OnLevelAdded(ULevel* InLevel, UWorld* InWorld);

	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<ARougePlayerStart>> CachedPlayerStarts;

private:
	void HandleOnActorSpawned(AActor* SpawnedActor);
};
