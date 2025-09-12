// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/FatedBrandAbilitySystemComponent.h"

void UFatedBrandAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag)) continue;

		TryActivateAbility(AbilitySpec.Handle);
	}
}

void UFatedBrandAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{

}