#pragma once

UENUM(BlueprintType)
enum class EFinisherCameraType : uint8
{
	EFCT_NoCineCamera UMETA(DisplayName="NoCineCamera"),
	EFCT_FinisherRig UMETA(DisplayName="FinisherRig"),
	EFCT_CameraRig UMETA(DisplayName="CameraRig"),
	
};
