// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameManager.h"

#include "Core/RougeDemoPlayerController.h"
#include "Interface/ControllerManagerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AGameManager::AGameManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	GameSetUp();
}



// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGameManager::GameSetUp()
{
	StartTimer();
}

void AGameManager::UpdateCharactersXP(float Percent, int32 Level)
{
	ARougeDemoPlayerController* RougeDemoPlayerController = Cast<ARougeDemoPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if(RougeDemoPlayerController == nullptr) { return; }
	if (IControllerManagerInterface* ControllerImpl = Cast<IControllerManagerInterface>(RougeDemoPlayerController))
	{
		ControllerImpl->UpdateLevelBar(Percent, Level);
	}
}

void AGameManager::PrepareLevelUp()
{
	ARougeDemoPlayerController* RougeDemoPlayerController = Cast<ARougeDemoPlayerController>((UGameplayStatics::GetPlayerController(GetWorld(), 0)));
	if(RougeDemoPlayerController == nullptr) { return; }
	RougeDemoPlayerController->OnLevelUp();

	//所有Controller
	/*for(const auto ControllerRef : PlayerControllerArr)
	{
		ControllerRef->OnLevelUp();
	}*/
}

void AGameManager::StartTimer()
{
	GetWorld()->GetTimerManager().SetTimer(
		UpdateTimerTimerHandle,
		this,
		&AGameManager::UpdateTimerCallBack,
		1.0f,
		true
	);
}

void AGameManager::EndGame()
{
	GetWorld()->GetTimerManager().ClearTimer(UpdateTimerTimerHandle);
}

void AGameManager::UpdateTimer()
{
	Time++;
	if(Time > 59)
	{
		Time = Time - 60;
		++Minutes;
	}else
	{
		FText S1 = Time < 10 ? FText::FromString(FString::FromInt(0)) : FText::FromString(TEXT(""));
		FText S2 = FText::FromString(FString::FromInt(Time));
		FText S = FText::Format(FText::FromString(TEXT("{0}{1}")), S1, S2);
		
		FText M = FText::FromString(FString::FromInt(Minutes));
		GameTime = FText::Format(FText::FromString(TEXT("{0}:{1}")), Minutes, S);

		
		if(Minutes > MaxGameTime)
		{
			EndGame();
		}
	}
}

void AGameManager::UpdateTimerCallBack()
{
	UpdateTimer();

	ARougeDemoPlayerController* RougeDemoPlayerController = Cast<ARougeDemoPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if(RougeDemoPlayerController == nullptr) { return; }
	if (IControllerManagerInterface* ControllerImpl = Cast<IControllerManagerInterface>(RougeDemoPlayerController))
	{
		ControllerImpl->UpdateTime(GameTime);
	}
}

