// Fill out your copyright notice in the Description page of Project Settings.


#include "Assets/RougeAssetManagerStartupJob.h"

TSharedPtr<FStreamableHandle> FRougeAssetManagerStartupJob::DoJob() const
{
	//任务开始时间
	const double JobStartTime = FPlatformTime::Seconds();
	
	TSharedPtr<FStreamableHandle> Handle;
	UE_LOG(LogTemp, Display, TEXT("Startup job \"%s\" starting"), *JobName);
	JobFunc(*this, Handle);

	if (Handle.IsValid())
	{
		Handle->BindUpdateDelegate(FStreamableUpdateDelegate::CreateRaw(this, &FRougeAssetManagerStartupJob::UpdateSubstepProgressFromStreamable));
		Handle->WaitUntilComplete(0.0f, false);
		Handle->BindUpdateDelegate(FStreamableUpdateDelegate());
	}

	UE_LOG(LogTemp, Display, TEXT("Startup job \"%s\" took %.2f seconds to complete"), *JobName, FPlatformTime::Seconds() - JobStartTime);

	return Handle;
}
