// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BaseAI.h"

#include "AI/BaseAIAnimInstance.h"
#include "Components/WidgetComponent.h"

// Sets default values
ABaseAI::ABaseAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TargetWidget =  CreateDefaultSubobject<UWidgetComponent>(TEXT("TargetWidget"));
	TargetWidget->SetupAttachment(GetMesh());
	HealthWidget =  CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidget"));
	HealthWidget->SetupAttachment(GetMesh());
	
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Tags.Add(FName("Enemy"));

	DissolveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DissolveTimelineComponent"));
}

// Called when the game starts or when spawned
void ABaseAI::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this,&ABaseAI::ReceiveDamage);
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

void ABaseAI::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatorController, AActor* DamageCauser)
{
	
	AttributeInfo.Health = FMath::Clamp(AttributeInfo.Health - Damage, 0.f, AttributeInfo.MaxHealth);

	UpdateHealthHUD();
	PlayHitReactMontage();

	if(AttributeInfo.Health == 0.f)
	{
		Dead();
	}
}

void ABaseAI::UpdateHealthHUD()
{
	SetHealthHUD(AttributeInfo.Health);
}

void ABaseAI::SetHealthHUD(float NewHealth)
{
	AttributeInfo.Health = NewHealth;
}

void ABaseAI::Elim()
{
	if(DissolveMaterialInstance)
	{
		//UMaterialInterface* DissolveMaterialInterface = GetMesh()->GetMaterial(0);
		DynamicDissolveMaterialInstance = UMaterialInstanceDynamic::Create(DissolveMaterialInstance,this);
		GetMesh()->SetMaterial(0,DynamicDissolveMaterialInstance);
		DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Dissolve"),0.55f);
		DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Glow"),35.f);
	}

	StartDissolve();
}

void ABaseAI::DestroyCallBack()
{
	Elim();
	//Destroy();

	GetWorld()->GetTimerManager().ClearTimer(DestroyTimerHandle);
}

void ABaseAI::UpdateDissolveMaterial(float DissolveValue)
{
	if(DynamicDissolveMaterialInstance)
	{
		DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Dissolve"),DissolveValue);
	}
}

void ABaseAI::StartDissolve()
{
	DissolveTrack.BindDynamic(this,&ABaseAI::UpdateDissolveMaterial);
	if(DissolveCurve && DissolveTimeline)
	{
		DissolveTimeline->AddInterpFloat(DissolveCurve,DissolveTrack);
		DissolveTimeline->Play();
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
	if(AttributeInfo.Health <= 0.f)
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

	//GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	//GetMesh()->SetCollisionResponseToChannel(ECC_PhysicsBody,ECR_Block);
	GetMesh()->SetCollisionObjectType(ECC_PhysicsBody);
	//模拟物理
	GetMesh()->SetSimulatePhysics(true);

	//2s后调用
	GetWorld()->GetTimerManager().SetTimer(
		DestroyTimerHandle,
		this,
		&ABaseAI::DestroyCallBack,
		2.f,
		false
	);

	Elim();
}

void ABaseAI::PlayHitReactMontage()
{
	UBaseAIAnimInstance* AnimInstance = Cast<UBaseAIAnimInstance>(GetMesh()->GetAnimInstance());
	if(AnimInstance)
	{
		if(F_HitMontage)
		{
			AnimInstance->Montage_Play(F_HitMontage);
		}
	}
}



