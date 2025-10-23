// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalc/MMC_VitalSurge.h"

#include "AbilitySystem/FatedBrandAttributeSet.h"
#include "Project_FatedBrand/Project_FatedBrand.h"

UMMC_VitalSurge::UMMC_VitalSurge()
{
	VitalSurgeDef.AttributeToCapture = UFatedBrandAttributeSet::GetVitalSurgeAttribute();
	VitalSurgeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	VitalSurgeDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VitalSurgeDef);
}

float UMMC_VitalSurge::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float VitalSurge = 0.f;
	GetCapturedAttributeMagnitude(VitalSurgeDef, Spec, EvaluationParameters, VitalSurge);

	return VitalSurge > 0.f ? 1.f : 0.f;	
}
