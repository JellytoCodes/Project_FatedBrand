// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "FatedBrandGameplayTags.h"
#include "AbilitySystem/FatedBrandAttributeSet.h"

struct FFatedBrandDamageStatics
{
	//DECLARE_ATTRIBUTE_CAPTUREDEF(Defense)

	FFatedBrandDamageStatics()
	{
		//DEFINE_ATTRIBUTE_CAPTUREDEF(UFatedBrandAttributeSet, Defense, Target, false)
	}
};

static const FFatedBrandDamageStatics& DamageStatics()
{
	static FFatedBrandDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	//RelevantAttributesToCapture.Add(DamageStatics().DefenseDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float Damage = 1.f;
	for (const TPair<FGameplayTag, float>& TagMagnitude : Spec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Value <= 0.f) continue;

		Damage = TagMagnitude.Value;
	}


	const FGameplayModifierEvaluatedData EvaluatedData(UFatedBrandAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
