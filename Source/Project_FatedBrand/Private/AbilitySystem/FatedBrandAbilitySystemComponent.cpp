// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/FatedBrandAbilitySystemComponent.h"

#include "FatedBrandFunctionLibrary.h"
#include "FatedBrandGameplayTags.h"
#include "AbilitySystem/Abilities/FatedBrandGameplayAbility.h"
#include "DataAssets/DataAsset_AbilityInfo.h"
#include "Game/FatedBrandSaveGame.h"

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
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void UFatedBrandAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UFatedBrandGameplayAbility>>& PassiveAbilities)
{
	for (const TSubclassOf<UFatedBrandGameplayAbility> AbilityClass : PassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbility(AbilitySpec);
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void UFatedBrandAbilitySystemComponent::AddCharacterAbilitiesFromSaveData(UFatedBrandSaveGame* SaveData)
{
	for (const FSavedAbility& Data : SaveData->SavedAbilities)
	{
		const TSubclassOf<UGameplayAbility> LoadedAbilityClass = Data.Ability;

		FGameplayAbilitySpec LoadedAbilitySpec = FGameplayAbilitySpec(LoadedAbilityClass, Data.LevelRequirement);
		LoadedAbilitySpec.GetDynamicSpecSourceTags().AddTag(Data.InputTag);
		LoadedAbilitySpec.GetDynamicSpecSourceTags().AddTag(Data.StatusTag);

		if (Data.AbilityType == FatedBrandGameplayTags::Abilities_Offensive_Type)
		{
			GiveAbility(LoadedAbilitySpec);
		}
		else if (Data.AbilityType == FatedBrandGameplayTags::Abilities_Passive_Type)
		{
			GiveAbility(LoadedAbilitySpec);

			// 추후 즉시 발동이 필요한 패시브가 존재할 경우 기능 추가
		}
		EquipAbility(Data.AbilityTag, Data.InputTag);
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void UFatedBrandAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) return;

	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
			else
			{
				const UGameplayAbility* Ability = AbilitySpec.GetPrimaryInstance();
				PRAGMA_DISABLE_DEPRECATION_WARNINGS
				const FPredictionKey PredictionKey = Ability == nullptr ? AbilitySpec.ActivationInfo.GetActivationPredictionKey() : Ability->GetCurrentActivationInfo().GetActivationPredictionKey();
				PRAGMA_DISABLE_DEPRECATION_WARNINGS
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, PredictionKey);
			}
		}
	}
}

void UFatedBrandAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) return;
	
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		AbilitySpecInputReleased(AbilitySpec);
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag) && AbilitySpec.IsActive())
		{
			const UGameplayAbility* Ability = AbilitySpec.GetPrimaryInstance();
			PRAGMA_DISABLE_DEPRECATION_WARNINGS
			const FPredictionKey PredictionKey = Ability == nullptr ? AbilitySpec.ActivationInfo.GetActivationPredictionKey() : Ability->GetCurrentActivationInfo().GetActivationPredictionKey();
			PRAGMA_DISABLE_DEPRECATION_WARNINGS
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, PredictionKey);
		}
	}
}

void UFatedBrandAbilitySystemComponent::OnAbilityInputHeld(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) return;

	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

bool UFatedBrandAbilitySystemComponent::OnActivatePassiveAbility(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		const bool IsEquippedStatus = GetStatusFromSpec(*AbilitySpec) == FatedBrandGameplayTags::Abilities_Status_Equipped;

		if (IsPassiveAbility(*AbilitySpec) && IsEquippedStatus)
		{
			return TryActivateAbility(AbilitySpec->Handle);
		}
	}
	return false;
}

void UFatedBrandAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock ActiveScopeLock(*this);

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
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
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
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

void UFatedBrandAbilitySystemComponent::ClearInputTag(FGameplayAbilitySpec* Spec)
{
	const FGameplayTag InputTag = GetInputTagFromSpec(*Spec);
	Spec->GetDynamicSpecSourceTags().RemoveTag(InputTag);
}

void UFatedBrandAbilitySystemComponent::AssignInputTagToAbility(FGameplayAbilitySpec& Spec, const FGameplayTag& InputTag)
{
	ClearInputTag(&Spec);
	Spec.GetDynamicSpecSourceTags().AddTag(InputTag);
}

bool UFatedBrandAbilitySystemComponent::InputTagIsEmpty(const FGameplayTag& InputTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilityHasInputTag(AbilitySpec, InputTag)) return false;
	}
	return true;
}

bool UFatedBrandAbilitySystemComponent::AbilityHasInputTag(const FGameplayAbilitySpec& Spec, const FGameplayTag& InputTag)
{
	return Spec.GetDynamicSpecSourceTags().HasTagExact(InputTag);
}

bool UFatedBrandAbilitySystemComponent::AbilityHasAnyInputTag(const FGameplayAbilitySpec& Spec)
{
	return Spec.GetDynamicSpecSourceTags().HasTag(FGameplayTag::RequestGameplayTag(FName("Input")));
}

bool UFatedBrandAbilitySystemComponent::IsPassiveAbility(const FGameplayAbilitySpec& Spec) const
{
	const UDataAsset_AbilityInfo* AbilityInfo = UFatedBrandFunctionLibrary::GetAbilityInfo(GetAvatarActor());
	const FGameplayTag AbilityTag = GetAbilityTagFromSpec(Spec);
	const FFatedBrandAbilityInfo& Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	const FGameplayTag AbilityType = Info.AbilityType;

	return AbilityType.MatchesTagExact(FatedBrandGameplayTags::Abilities_Passive_Type);
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

FGameplayAbilitySpec* UFatedBrandAbilitySystemComponent::GetSpecWithNebulaSlot(const FGameplayTag& InputTag)
{
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			return &AbilitySpec;
		}
	}
	return nullptr;
}

void UFatedBrandAbilitySystemComponent::UpdateAbilityStatuses(const FGameplayTag& AbilityTag)
{
	UDataAsset_AbilityInfo* AbilityInfo = UFatedBrandFunctionLibrary::GetAbilityInfo(GetAvatarActor());

	for (FFatedBrandAbilityInfo& Info : AbilityInfo->AbilityInformation)
	{
		if (Info.AbilityTag == AbilityTag && GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, 1);
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(FatedBrandGameplayTags::Abilities_Status_Unlocked);
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec);

			AbilityStatusChanged.Broadcast(AbilityTag, FatedBrandGameplayTags::Abilities_Status_Unlocked);
		}
	}
}

void UFatedBrandAbilitySystemComponent::EquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		const FGameplayTag& PrevInputTag = GetInputTagFromSpec(*AbilitySpec);
		const FGameplayTag& Status = GetStatusFromSpec(*AbilitySpec);

		const bool bStatusValid = Status == FatedBrandGameplayTags::Abilities_Status_Equipped || Status == FatedBrandGameplayTags::Abilities_Status_Unlocked;

		if (bStatusValid)
		{
			if (!InputTagIsEmpty(InputTag))
			{
				FGameplayAbilitySpec* SpecWithInputTag = GetSpecWithNebulaSlot(InputTag);
				if (SpecWithInputTag)
				{
					if (AbilityTag.MatchesTagExact(GetAbilityTagFromSpec(*SpecWithInputTag)))
					{
						AbilityEquipped.Broadcast(AbilityTag, FatedBrandGameplayTags::Abilities_Status_Equipped, InputTag, PrevInputTag);
						return;
					}	
				}
				ClearInputTag(SpecWithInputTag);
			}
			if (!AbilityHasAnyInputTag(*AbilitySpec))
			{
				AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(GetStatusFromSpec(*AbilitySpec));
				AbilitySpec->GetDynamicSpecSourceTags().AddTag(FatedBrandGameplayTags::Abilities_Status_Equipped);
			}

			if (!IsPassiveAbility(*AbilitySpec))
			{
				AssignInputTagToAbility(*AbilitySpec, InputTag);
				MarkAbilitySpecDirty(*AbilitySpec);	
			}
		}
		AbilityEquipped.Broadcast(AbilityTag, FatedBrandGameplayTags::Abilities_Status_Equipped, InputTag, PrevInputTag);
	}
}

void UFatedBrandAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast();
	}
}
