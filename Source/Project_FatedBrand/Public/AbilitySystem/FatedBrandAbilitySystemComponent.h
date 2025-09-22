// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "FatedBrandAbilitySystemComponent.generated.h"

class UFatedBrandGameplayAbility;

DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);

UCLASS()
class PROJECT_FATEDBRAND_API UFatedBrandAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public :
	void AddCharacterActivateAbilities(const TArray<TSubclassOf<UFatedBrandGameplayAbility>>& ActivateAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UFatedBrandGameplayAbility>>& PassiveAbilities);
	bool bStartupAbilitiesGiven = false;

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	void OnAbilityInputPressed(const FGameplayTag& InInputTag);
	void OnAbilityInputReleased(const FGameplayTag& InInputTag);
	void ForEachAbility(const FForEachAbility& Delegate);

	FGameplayTag GetStatusFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayTag GetSlotFromAbilityTag(const FGameplayTag& AbilityTag);

	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayAbilitySpec* GetSpecWithNebulaSlot(const FGameplayTag& NebulaSlot);

	void EquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& Slot);
};
