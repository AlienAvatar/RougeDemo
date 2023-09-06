// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystem;
class UParticleSystemComponent;

UCLASS()
class ROUGEDEMO_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* ParticleSystemComp;
	
	//当Projectile在这个距离内，寻敌行为停止
	UPROPERTY(EditDefaultsOnly)
	float DistanceToStopHomingBehavior = 200.f;

	UPROPERTY(EditDefaultsOnly)
	float LifespanAfterReachingTarget = 2.f;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* HitParticle;

	UFUNCTION()
	void OnProjectileStop(const FHitResult& ImpactResult);

	UPROPERTY(EditDefaultsOnly)
	float BaseDamage = 30.f;
private:
	void SetIgnoreActor(AActor* IgnoreActor);

	void DestroyProjectile();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
