// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/ReceiveDamageLibrary.h"

#include "Enum/EMovementDirection.h"
#include "Kismet/KismetMathLibrary.h"

EMovementDirection UReceiveDamageLibrary::CalculateDamageCauserDirection(AActor* DamagedActor, AActor* DamageCauser)
{
	FVector DamagedFwdVector = DamagedActor->GetActorForwardVector();
	
	const FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(DamagedActor->GetActorLocation(),DamageCauser->GetActorLocation());
	const FVector LocCauseDir = UKismetMathLibrary::Quat_UnrotateVector(FQuat(LookRotation),DamagedFwdVector);
	
	const float CauseSum = FMath::Abs(LocCauseDir.X) +  FMath::Abs(LocCauseDir.Y) +  FMath::Abs(LocCauseDir.Z);
	FVector CauseDirection = LocCauseDir / CauseSum;
	//FVector CauserFwdVector = DamageCauser->GetActorForwardVector();
	//const FVector LocLookDir = UKismetMathLibrary::Quat_UnrotateVector(FQuat(LookRotation),CauserFwdVector);
	//const float Sum = FMath::Abs(LocLookDir.X) +  FMath::Abs(LocLookDir.Y) +  FMath::Abs(LocLookDir.Z);
	//FVector RelativeDirection = LocLookDir / Sum;
	//UE_LOG(LogTemp, Warning, TEXT("RelativeDirection[%s]"), *RelativeDirection.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("CauseDirection[%s]"), *CauseDirection.ToString());

	//CauseDirection.X > 0.f 面向攻击者,反之 背向攻击者
	//CauseDirection.X > 0.5f 正面面向
	if(UKismetMathLibrary::InRange_FloatFloat(CauseDirection.X,0.5f,1.f,true,true))
	{
		return EMovementDirection::EMD_Forward;
	//CauseDirection.X < -0.5f 正面背向
	}else if(UKismetMathLibrary::InRange_FloatFloat(CauseDirection.X,-1.f,-0.5f,true,true))
	{
		return EMovementDirection::EMD_Backward;
	//CauseDirection.X > 0.f 正面侧向	
	}else if(UKismetMathLibrary::InRange_FloatFloat(CauseDirection.X,0.f,0.5f,true,true))
	{
		//CauseDirection.Y > 0.f 左边
		if(CauseDirection.Y >= 0.f)
		{
			return EMovementDirection::EMD_Left;
		}else
		{
			return EMovementDirection::EMD_Right;
		}
	}else if(UKismetMathLibrary::InRange_FloatFloat(CauseDirection.X,-0.5f,0.f,true,true))
	{
		//右边
		//CauseDirection.Y > 0.f 左边
		if(CauseDirection.Y >= 0.f)
		{
			return EMovementDirection::EMD_Left;
		}else
		{
			return EMovementDirection::EMD_Right;
		}
	}
	else
	{
		return EMovementDirection::EMD_MAX;
	}
}
