// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameManager.h"

#include "Core/RougeDemoPlayerController.h"
#include "Interface/ControllerManagerInterface.h"
#include "Kismet/GameplayStatics.h"


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
	
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGameManager::UpdateCharactersXP(float Percent, int32 Level)
{
	ARougeDemoPlayerController* RougeDemoPlayerController = Cast<ARougeDemoPlayerController>((UGameplayStatics::GetPlayerController(GetWorld(), 0)));
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

