// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Executions/RougeDamageMagnitudeCalculation.h"

#include "GameplayEffectExecutionCalculation.h"
#include "AbilitySystem/Attribute/RougeCombatSet.h"

struct SDamageStatics
{
	//定义对象
	DECLARE_ATTRIBUTE_CAPTUREDEF(BaseDamage);

	SDamageStatics()
	{
		//尝试对象赋值
		DEFINE_ATTRIBUTE_CAPTUREDEF(URougeCombatSet, BaseDamage, Source, true);
	}
};

static const SDamageStatics& DamageStatics()
{
	static SDamageStatics DStatics;
	return DStatics;
}

URougeDamageMagnitudeCalculation::URougeDamageMagnitudeCalculation()
{
}

float URougeDamageMagnitudeCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Damage = 0.0f;
	//判断是否成功获取了对应的属性值
	GetCapturedAttributeMagnitude(DamageStatics().BaseDamageDef, Spec, EvaluationParameters, Damage);

	return Damage;
}
