// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/GameFeatureAction_AddGameplayCuePath.h"
#define LOCTEXT_NAMESPACE "GameFeatures"

UGameFeatureAction_AddGameplayCuePath::UGameFeatureAction_AddGameplayCuePath()
{
	DirectoryPathsToAdd.Add(FDirectoryPath{ TEXT("/GameplayCues") });
}

#if WITH_EDITOR
EDataValidationResult UGameFeatureAction_AddGameplayCuePath::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult Result = Super::IsDataValid(ValidationErrors);

	FText ErrorReason = FText::GetEmpty();
	for (const FDirectoryPath& Directory : DirectoryPathsToAdd)
	{
		if (Directory.Path.IsEmpty())
		{
			const FText InvalidCuePathError = FText::Format(LOCTEXT("InvalidCuePathError", "'{0}' is not a valid path!"), FText::FromString(Directory.Path));
			ValidationErrors.Emplace(InvalidCuePathError);
			ValidationErrors.Emplace(ErrorReason);
			Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);
		}
	}

	return CombineDataValidationResults(Result, EDataValidationResult::Valid);
}
#endif	// WITH_EDITOR

#undef LOCTEXT_NAMESPACE
