// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BaseAI.h"

#include "Components/WidgetComponent.h"

// Sets default values
ABaseAI::ABaseAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TargetWidget =  CreateDefaultSubobject<UWidgetComponent>(TEXT("TargetWidget"));
	TargetWidget->SetupAttachment(GetMesh());

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Tags.Add(FName("Enemy"));
}

// Called when the game starts or when spawned
void ABaseAI::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Velocity = GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();
}

// Called to bind functionality to input
void ABaseAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseAI::ToggleMarket(bool bLockOn)
{
	if(TargetWidget)
	{
		TargetWidget->SetVisibility(bLockOn);
	}
}

float ABaseAI::OnTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation,
	UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, AActor* DamageCauser,
	AController* InstigatedByController, AActor* DamageCauserActor)
{
	UE_LOG(LogTemp,Warning,TEXT("Damage[%f]"),Damage);
	return 0.f;
}

float ABaseAI::InternalTakePointDamage(float Damage, FPointDamageEvent const& PointDamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp,Warning,TEXT("Damage[%f]"),Damage);
	AttributeInfo.Health = AttributeInfo.Health - Damage;
	if(AttributeInfo.Health < 0.f)
	{
		Dead();
	}
	
	return Super::InternalTakePointDamage(Damage, PointDamageEvent, EventInstigator, DamageCauser);
}

void ABaseAI::Dead()
{
	//解除锁定
	ToggleMarket(false);

	State = EState::ES_Dead;

	
	//模拟物理
	GetMesh()->SetSimulatePhysics(true);

	
}



