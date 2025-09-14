// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/FatedBrandAttributeSet.h"

#include "GameplayEffectExtension.h"

UFatedBrandAttributeSet::UFatedBrandAttributeSet()
{
}

void UFatedBrandAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());

		SetCurrentHealth(NewCurrentHealth);
	}
}
