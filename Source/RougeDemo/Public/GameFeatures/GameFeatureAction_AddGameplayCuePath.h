// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "GameFeatureAction_AddGameplayCuePath.generated.h"

/**
 * 
 */
UCLASS()
class ROUGEDEMO_API UGameFeatureAction_AddGameplayCuePath : public UGameFeatureAction
{
	GENERATED_BODY()

public:

	UGameFeatureAction_AddGameplayCuePath();

	//~UObject interface
	#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif
	//~End of UObject interface

	const TArray<FDirectoryPath>& GetDirectoryPathsToAdd() const { return DirectoryPathsToAdd; }

private:
	/** List of paths to register to the gameplay cue manager. These are relative tot he game content directory */
	UPROPERTY(EditAnywhere, Category = "Game Feature | Gameplay Cues", meta = (RelativeToGameContentDir, LongPackageName))
	TArray<FDirectoryPath> DirectoryPathsToAdd;
};
