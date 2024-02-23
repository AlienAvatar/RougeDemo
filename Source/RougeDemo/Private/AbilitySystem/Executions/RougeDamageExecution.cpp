// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Executions/RougeDamageExecution.h"

#include "AbilitySystem/RougeGameplayEffectContext.h"
#include "AbilitySystem/Attribute/RougeCombatSet.h"
#include "AbilitySystem/Attribute/RougeHealthSet.h"

struct FDamageStatics
{
	FGameplayEffectAttributeCaptureDefinition BaseDamageDef;

	FDamageStatics()
	{
		BaseDamageDef = FGameplayEffectAttributeCaptureDefinition(URougeCombatSet::GetBaseDamageAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	}
};

static FDamageStatics& DamageStatics()
{
	static FDamageStatics Statics;
	return Statics;
}
URougeDamageExecution::URougeDamageExecution()
{
	RelevantAttributesToCapture.Add(DamageStatics().BaseDamageDef);
}

void URougeDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
#if WITH_SERVER_CODE
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FRougeGameplayEffectContext* TypedContext = FRougeGameplayEffectContext::ExtractEffectContext(Spec.GetContext());
	check(TypedContext);

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float BaseDamage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BaseDamageDef, EvaluateParameters, BaseDamage);

	if(TypedContext != nullptr)
	{
		const AActor* EffectCauser = TypedContext->GetEffectCauser();
		const FHitResult* HitActorResult = TypedContext->GetHitResult();

		AActor* HitActor = nullptr;
		FVector ImpactLocation = FVector::ZeroVector;
		FVector ImpactNormal = FVector::ZeroVector;
		FVector StartTrace = FVector::ZeroVector;
		FVector EndTrace = FVector::ZeroVector;

		if (HitActorResult)
		{
			const FHitResult& CurHitResult = *HitActorResult;
			HitActor = CurHitResult.HitObjectHandle.FetchActor();
			if (HitActor)
			{
				ImpactLocation = CurHitResult.ImpactPoint;
				ImpactNormal = CurHitResult.ImpactNormal;
				StartTrace = CurHitResult.TraceStart;
				EndTrace = CurHitResult.TraceEnd;
			}
		}

		UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
		if (!HitActor)
		{
			HitActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor_Direct() : nullptr;
			if (HitActor)
			{
				ImpactLocation = HitActor->GetActorLocation();
			}
		}

		//是否是一个队伍
		/*float DamageInteractionAllowedMultiplier = 0.0f;
		if (HitActor)
		{
			URougeTeamSubsystem* TeamSubsystem = HitActor->GetWorld()->GetSubsystem<URougeTeamSubsystem>();
			DamageInteractionAllowedMultiplier = TeamSubsystem->CanCauseDamage(EffectCauser, HitActor) ? 1.0 : 0.0;
		}*/

		const float DamageDone = FMath::Max(BaseDamage, 0.0f);

		if (DamageDone > 0.0f)
		{
			// Apply a damage modifier, this gets turned into - health on the target
			OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(URougeHealthSet::GetDamageAttribute(), EGameplayModOp::Additive, DamageDone));
		}
	}
#endif 
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
}
