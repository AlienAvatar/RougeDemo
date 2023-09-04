// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Action.h"
#include "Components/ActionComponent.h"

UActionComponent* UAction::GetOwningComponent() const
{
	return ActionComp;
}



void UAction::Initialize(UActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

bool UAction::IsRunning() const
{
	return RepData.bIsRunning;
}

void UAction::StopAction_Implementation(AActor* Instigator)
{
}

void UAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Started: %s"), *GetNameSafe(this));

	UActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	
}

bool UAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}

	UActionComponent* Comp2 = GetOwningComponent();

	//是否有Block Tag打断当前Tag
	/*if (Comp2->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}*/

	return true;
}
