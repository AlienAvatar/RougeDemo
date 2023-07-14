// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enum/EMovementState.h"
#include "RougeDemoCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class ROUGEDEMO_API ARougeDemoCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARougeDemoCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void OnBeginPlay();
private:

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* FollowCamera;

	float AO_Pitch;
	float AO_Yaw;

	FRotator CurrentRotation;
	FRotator StartRotation;
	void AimOffset(float DeltaTime);

	void SetEssentialValues(float DeltaTime);
	
	FVector CalculateAcceleration();

	void CacheValues();

	FVector PreviousVelocity;

	//加速度
	FVector Acceleration;

	EMovementState MovementState;

	bool bIsMoving;

	float Speed;

	FRotator LastVelocityRotation;

	float MovementInputAmount;

	bool bHasMovementInput;

	FRotator LastMovementInputRotation;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE FVector GetAcceleration() const { return Acceleration; }
	FORCEINLINE EMovementState GetMovementState() const { return MovementState; }
	FORCEINLINE bool GetIsMoving() const { return bIsMoving; }
	FORCEINLINE bool GetHasMovementInput() const { return bHasMovementInput; }
	
};
