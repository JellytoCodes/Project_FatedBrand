// Fill out your copyright notice in the Description page of Project Settings.

#include "DataAssets/DataAsset_StartUpDataBase.h"
#include "AbilitySystem/Abilities/FatedBrandGameplayAbility.h"
#include "AbilitySystem/FatedBrandAbilitySystemComponent.h"

void UDataAsset_StartUpDataBase::InitializeGameplayEffect(UFatedBrandAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	check(InASCToGive);

	if (!StartUpGameplayEffects.IsEmpty())
	{
		for (const TSubclassOf<UGameplayEffect>& EffectClass : StartUpGameplayEffects)
		{
			if (!EffectClass) continue;
			UGameplayEffect* EffectCDO = EffectClass->GetDefaultObject<UGameplayEffect>();
			InASCToGive->ApplyGameplayEffectToSelf(EffectCDO, ApplyLevel, InASCToGive->MakeEffectContext());
		}
	}
}