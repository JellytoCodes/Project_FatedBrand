// Fill out your copyright notice in the Description page of Project Settings.

#include "DataAssets/DataAsset_StartUpDataBase.h"
#include "AbilitySystem/FatedBrandAbilitySystemComponent.h"

void UDataAsset_StartUpDataBase::InitializeGameplayEffect(UFatedBrandAbilitySystemComponent* InASCToGive, EStartUpCharacterName StartUpCharacterName, int32 ApplyLevel)
{
	check(InASCToGive);

	if (!StartUpAttributeInfo.IsEmpty())
	{
		for (const FFatedBrandStartUpAttributeInfo& AttributeInfo : StartUpAttributeInfo)
		{
			if (!AttributeInfo.StartUpGameplayEffect || AttributeInfo.StartUpCharacterName != StartUpCharacterName) continue;
			UGameplayEffect* EffectCDO = AttributeInfo.StartUpGameplayEffect->GetDefaultObject<UGameplayEffect>();
			InASCToGive->ApplyGameplayEffectToSelf(EffectCDO, ApplyLevel, InASCToGive->MakeEffectContext());
		}
	}
}