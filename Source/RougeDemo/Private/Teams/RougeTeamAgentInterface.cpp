// Fill out your copyright notice in the Description page of Project Settings.


#include "Teams/RougeTeamAgentInterface.h"

URougeTeamAgentInterface::URougeTeamAgentInterface(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}
// Add default functionality here for any IRougeTeamAgentInterface functions that are not pure virtual.
void IRougeTeamAgentInterface::ConditionalBroadcastTeamChanged(TScriptInterface<IRougeTeamAgentInterface> This,
	FGenericTeamId OldTeamID, FGenericTeamId NewTeamID)
{
	if (OldTeamID != NewTeamID)
	{
		const int32 OldTeamIndex = GenericTeamIdToInteger(OldTeamID); 
		const int32 NewTeamIndex = GenericTeamIdToInteger(NewTeamID);

		UObject* ThisObj = This.GetObject();
		//UE_LOG(LogTemp, Verbose, TEXT("[%s] %s assigned team %d"), *GetClientServerContextString(ThisObj), *GetPathNameSafe(ThisObj), NewTeamIndex);

		This.GetInterface()->GetTeamChangedDelegateChecked().Broadcast(ThisObj, OldTeamIndex, NewTeamIndex);
	}
}
