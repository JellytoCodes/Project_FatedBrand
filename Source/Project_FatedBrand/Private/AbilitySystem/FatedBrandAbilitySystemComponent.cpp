// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/FatedBrandAbilitySystemComponent.h"

#include "FatedBrandFunctionLibrary.h"
#include "FatedBrandGameplayTags.h"
#include "AbilitySystem/Abilities/FatedBrandGameplayAbility.h"
#include "DataAssets/DataAsset_AbilityInfo.h"
#include "Project_FatedBrand/Project_FatedBrand.h"

void UFatedBrandAbilitySystemComponent::AddCharacterActivateAbilities(const TArray<TSubclassOf<UFatedBrandGameplayAbility>>& ActivateAbilities)
{
	for (const TSubclassOf<UFatedBrandGameplayAbility> AbilityClass : ActivateAbilities)
	{
		const UFatedBrandGameplayAbility* FatedBrandAbility = AbilityClass.GetDefaultObject();
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		
		if (FatedBrandAbility && FatedBrandAbility->StartupInputTag.IsValid())
		{
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(FatedBrandAbility->StartupInputTag);
		}
		GiveAbility(AbilitySpec);
	}
}

void UFatedBrandAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UFatedBrandGameplayAbility>>& PassiveAbilities)
{
	for (const TSubclassOf<UFatedBrandGameplayAbility> AbilityClass : PassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

FGameplayTag UFatedBrandAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.GetDynamicSpecSourceTags())
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Input"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UFatedBrandAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.GetDynamicSpecSourceTags())
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UFatedBrandAbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.GetDynamicSpecSourceTags())
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

void UFatedBrandAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag)) continue;

		AbilitySpecInputPressed(AbilitySpec);
		TryActivateAbility(AbilitySpec.Handle);
		if (AbilitySpec.IsActive())
		{
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
		}
	}
}

void UFatedBrandAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		AbilitySpec.InputPressed = false;

		if (!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag) && AbilitySpec.IsActive())
		{
			AbilitySpecInputReleased(AbilitySpec);
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
		}
	}
}

void UFatedBrandAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			
		}
	}
}

FGameplayTag UFatedBrandAbilitySystemComponent::GetStatusFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetStatusFromSpec(*Spec);
	}
	return FGameplayTag();
}

FGameplayTag UFatedBrandAbilitySystemComponent::GetSlotFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetInputTagFromSpec(*Spec);
	}
	return FGameplayTag();
}

FGameplayAbilitySpec* UFatedBrandAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

FGameplayAbilitySpec* UFatedBrandAbilitySystemComponent::GetSpecWithNebulaSlot(const FGameplayTag& NebulaSlot)
{
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(NebulaSlot))
		{
			return &AbilitySpec;
		}
	}
	return nullptr;
}

void UFatedBrandAbilitySystemComponent::UpdateAbilityStatuses(const FGameplayTag& AbilityTag)
{
	UDataAsset_AbilityInfo* AbilityInfo = UFatedBrandFunctionLibrary::GetAbilityInfo(GetAvatarActor());

	for (const FFatedBrandAbilityInfo& Info : AbilityInfo->AbilityInformation)
	{
		if (AbilityTag != Info.AbilityTag || !Info.AbilityTag.IsValid()) continue;

		if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, 1);
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(FatedBrandGameplayTags::Abilities_Status_Eligible);
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec);
		}
	}
}

void UFatedBrandAbilitySystemComponent::EquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& Slot)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		TryActivateAbility(AbilitySpec->Handle);
	}
}
