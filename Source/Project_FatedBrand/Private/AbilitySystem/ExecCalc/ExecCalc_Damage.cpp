// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "FatedBrandGameplayTags.h"
#include "AbilitySystem/FatedBrandAttributeSet.h"

struct FFatedBrandDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)

	FFatedBrandDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UFatedBrandAttributeSet, AttackPower, Source, false)
	}
};

static const FFatedBrandDamageStatics& DamageStatics()
{
	static FFatedBrandDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().AttackPowerDef);
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

	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackPowerDef, EvaluationParameters, SourceAttackPower);

	bool HasJugularRip = SourceASC->HasMatchingGameplayTag(FatedBrandGameplayTags::Ability_Activate_JugularRip);

	float CriticalHit = HasJugularRip && FMath::RandRange(1,100) < 50 ? 2.f : 1.f;

	float Damage = 1.f;
	for (const TPair<FGameplayTag, float>& TagMagnitude : Spec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Value <= 0.f) continue;

		Damage = (TagMagnitude.Value * SourceAttackPower);
	}

	const FGameplayModifierEvaluatedData EvaluatedData(UFatedBrandAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
