// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/EnemyAIInterface.h"

// Add default functionality here for any IEnemyAIInterface functions that are not pure virtual.
bool IEnemyAIInterface::SetMovementSpeed(EGait Gait)
{
	return false;
}

float IEnemyAIInterface::GetIdealRange()
{
	return 0.f;
}
