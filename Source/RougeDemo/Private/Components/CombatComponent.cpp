// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatComponent.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UCombatComponent::Attack()
{
	//是否在空中或者飞行状态
	//若是，进行空中攻击

	
	//若否，进行地面攻击
	//判断是否可以攻击，如在某些状态下不能攻击

	//判断是否正在攻击，是否处于攻击间隔中

	//检查武器Slot

	//判断攻击类型 物理，魔法

	
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

