// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Struct/AttributeInfo.h"
#include "Enum/EState.h"
#include "BaseAI.generated.h"

class UWidgetComponent;

UCLASS()
class ROUGEDEMO_API ABaseAI : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseAI();

	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* TargetWidget;

	bool bCanBeExecuted = true;

	bool NormalEnemies;

	bool BossEnemies;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=State)
	FAttributeInfo AttributeInfo;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ToggleMarket(bool bLockOn);

	FORCEINLINE float GetSpeed() const { return Speed; }
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=State)
	EState State;

	UPROPERTY(BlueprintReadWrite)
	float Speed;

	UPROPERTY(BlueprintReadWrite)
	FVector Velocity;
private:
	//受击
	virtual float OnTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, AActor* DamageCauser, AController* InstigatedByController, AActor* DamageCauserActor);

	virtual float InternalTakePointDamage(float Damage, FPointDamageEvent const& PointDamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	void Dead();


};
