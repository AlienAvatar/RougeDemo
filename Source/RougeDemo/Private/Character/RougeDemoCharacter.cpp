// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RougeDemoCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ARougeDemoCharacter::ARougeDemoCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//设置Camera
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 315.f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0.f,75.f,0.f);
	CameraBoom->SetRelativeLocation(FVector(0.f,0.f,110.f));
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);

	MovementState = EMovementState::EMS_Grounded;
}

// Called when the game starts or when spawned
void ARougeDemoCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARougeDemoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimOffset(DeltaTime);
	SetEssentialValues(DeltaTime);
}

void ARougeDemoCharacter::MoveForward(float Value)
{
	if(Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f,Controller->GetControlRotation().Yaw,0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(2 * Direction,Value);	 
	}
}

void ARougeDemoCharacter::MoveRight(float Value)
{
	if(Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f,Controller->GetControlRotation().Yaw,0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(2 * Direction,Value);
	}
}

void ARougeDemoCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ARougeDemoCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ARougeDemoCharacter::OnBeginPlay()
{
	//确保动画更新在Character后
	GetMesh()->AddTickPrerequisiteActor(this);
}

void ARougeDemoCharacter::AimOffset(float DeltaTime)
{
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();
	if(Speed == 0.f)
	{
		//LookDirection
		CurrentRotation = FRotator(0.f,GetBaseAimRotation().Yaw,0.f);
		AO_Yaw = UKismetMathLibrary::NormalizedDeltaRotator(CurrentRotation, StartRotation).Yaw;
		bUseControllerRotationYaw = false;
	}
	//runing
	if(Speed > 0.f)
	{
		StartRotation = FRotator(0.f,GetBaseAimRotation().Yaw,0.f);
		AO_Yaw = 0.f;
		bUseControllerRotationYaw = true;
	}
	AO_Pitch = GetBaseAimRotation().Pitch;
}

void ARougeDemoCharacter::SetEssentialValues(float DeltaTime)
{
	//计算加速度
	Acceleration = CalculateAcceleration();

	Speed = GetVelocity().Size();
	bIsMoving = Speed > 1.f;
	if(bIsMoving)
	{
		LastVelocityRotation = FRotationMatrix::MakeFromX(GetVelocity()).Rotator();
	}

	MovementInputAmount = GetCharacterMovement()->GetCurrentAcceleration().Size() / GetCharacterMovement()->GetMaxAcceleration();
	bHasMovementInput = MovementInputAmount > 0.f;
	if(MovementInputAmount > 0.f)
	{
		LastMovementInputRotation = FRotationMatrix::MakeFromX(GetCharacterMovement()->GetCurrentAcceleration()).Rotator();
	}
}

FVector ARougeDemoCharacter::CalculateAcceleration()
{
	//根据速度公式 delta Velocity / delta time = acceleration
	return (GetVelocity() - PreviousVelocity) / UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
}

void ARougeDemoCharacter::CacheValues()
{
	PreviousVelocity = GetVelocity();
}


// Called to bind functionality to input
void ARougeDemoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&ARougeDemoCharacter::Jump);
	
	
	PlayerInputComponent->BindAxis("MoveForward",this,&ARougeDemoCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&ARougeDemoCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn",this,&ARougeDemoCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp",this,&ARougeDemoCharacter::LookUp);
}

