// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_VitalSurge.generated.h"

UCLASS()
class PROJECT_FATEDBRAND_API UMMC_VitalSurge : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public :
	UMMC_VitalSurge();
	float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private :
	FGameplayEffectAttributeCaptureDefinition VitalSurgeDef;
};
