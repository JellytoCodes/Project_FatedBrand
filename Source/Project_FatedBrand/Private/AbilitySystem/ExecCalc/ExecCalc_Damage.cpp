// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

struct FFatedBrandDamageStatics
{
	
};

static const FFatedBrandDamageStatics& DamageStatics()
{
	static FFatedBrandDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
}
