// Fill out your copyright notice in the Description page of Project Settings.

#include "DataAssets/DataAsset_PlayerStartUpData.h"

#include "AbilitySystem/FatedBrandAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/FatedBrandGameplayAbility.h"

bool FFatedBrandAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}

void UDataAsset_PlayerStartUpData::GiveToAbilitySystemComponent(UFatedBrandAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);

	for (const FFatedBrandAbilitySet& AbilitySet : CharacterStartUpAbilitySets)
	{
		if (!AbilitySet.IsValid()) continue;

		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

		InASCToGive->GiveAbility(AbilitySpec);
	}
}
