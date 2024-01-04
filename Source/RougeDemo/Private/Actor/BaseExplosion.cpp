// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/BaseExplosion.h"

#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Lib/RougeDemoFunctionLibary.h"
#include "RougeDemo/RougeDemo.h"


// Sets default values
ABaseExplosion::ABaseExplosion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseExplosion::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseExplosion::PlayVFX()
{
	if(DamageFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			DamageFX,
			GetActorLocation()
		);
	}
	
}

void ABaseExplosion::SetUp(float pDamage, float pRadius, UNiagaraSystem* pDamageFX)
{
	Damage = pDamage;
	Radius = pRadius;
	DamageFX = pDamageFX;
}

void ABaseExplosion::DoWork()
{
	PlayVFX();
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeQueryArr;
	const EObjectTypeQuery EnemyObjectTypeQuery = UEngineTypes::ConvertToObjectType(ECC_TRACE_ENEMY);
	ObjectTypeQueryArr.Add(EnemyObjectTypeQuery);
	TArray<AActor*> IgnoreActorArr;
	TArray<AActor*> EnemyActorArr;
	TArray<FHitResult> OutHits;
	UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(),
		GetActorLocation(),
		GetActorLocation(),
		Radius,
		ObjectTypeQueryArr,
		false,
		IgnoreActorArr,
		DebugTraceType,
		OutHits,
		true
	);

	//Apply Damage
	// URougeDemoFunctionLibary::DamageEnemiesOnce(
	// 	OutHits,
	// 	Damage,
	// 	GetInstigator()->GetInstigatorController(),
	// 	this
	// );

	//Destroy
	SetLifeSpan(0.5f);
}
