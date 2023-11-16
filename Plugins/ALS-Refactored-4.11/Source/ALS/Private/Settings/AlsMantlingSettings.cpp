#include "Settings/AlsMantlingSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AlsMantlingSettings)

#if WITH_EDITOR
void FAlsGeneralMantlingSettings::PostEditChangeProperty(const FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.GetPropertyName() != GET_MEMBER_NAME_CHECKED(FAlsGeneralMantlingSettings, MantlingTraceObjectTypes))
	{
		return;
	}

	MantlingTraceResponses.SetAllChannels(ECR_Ignore);

	for (const auto ObjectType : MantlingTraceObjectTypes)
	{
		MantlingTraceResponses.SetResponse(UEngineTypes::ConvertToCollisionChannel(ObjectType), ECR_Block);
	}
}
#endif
