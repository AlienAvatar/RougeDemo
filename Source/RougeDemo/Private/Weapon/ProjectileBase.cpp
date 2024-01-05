// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ProjectileBase.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	RootComponent = SphereComp;

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMoveComp");
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->bInitialVelocityInLocalSpace = true;
	ProjectileMovementComp->ProjectileGravityScale = 0.0f;
	ProjectileMovementComp->InitialSpeed = 400.f;

	ParticleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystemComp");
	ParticleSystemComp->SetupAttachment(SphereComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	SetIgnoreActor(GetInstigator());
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(ProjectileMovementComp->HomingTargetComponent.IsValid())
	{
		float Distance = UKismetMathLibrary::Vector_Distance(
			GetActorLocation(),
			ProjectileMovementComp->HomingTargetComponent->GetComponentLocation());
		//当目标距离小于DistanceToStopHomingBehavior，停止寻敌
		if(Distance < DistanceToStopHomingBehavior)
		{
			ProjectileMovementComp->bIsHomingProjectile = false;
			SetLifeSpan(LifespanAfterReachingTarget);
		}
	}
}

void AProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentHit.AddDynamic(this, &AProjectileBase::OnActorHit);
	ProjectileMovementComp->OnProjectileStop.AddDynamic(this, &AProjectileBase::OnProjectileStop);
}

void AProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(HitFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			HitFX,
			Hit.Location
		);
	}
	
	
}

void AProjectileBase::OnProjectileStop(const FHitResult& ImpactResult)
{
	AActor* DamagedActor = ImpactResult.GetActor();
	if(DamagedActor)
	{
		UGameplayStatics::ApplyDamage(DamagedActor,BaseDamage,GetInstigatorController(),this, UDamageType::StaticClass());
		DestroyProjectile();
	}
}

void AProjectileBase::SetIgnoreActor(AActor* IgnoreActor)
{
	SphereComp->IgnoreComponentWhenMoving(SphereComp,true);
}

void AProjectileBase::DestroyProjectile()
{
	if(HitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),HitParticle,GetActorLocation(),GetActorRotation());
		Destroy();
	}
}

void AProjectileBase::Setup(UNiagaraSystem* pHitFX, float pDamage)
{
	HitFX = pHitFX;
	BaseDamage = pDamage;
}



