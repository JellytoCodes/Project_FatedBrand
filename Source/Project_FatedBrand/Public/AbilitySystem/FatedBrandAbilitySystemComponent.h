// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "FatedBrandAbilitySystemComponent.generated.h"

struct FFatedBrandAbilityInfo;
class UFatedBrandGameplayAbility;

DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEquipped, const FGameplayTag& /*Ability Tag*/, const FGameplayTag& /*Status Tag*/, const FGameplayTag& /*Input Tag*/, const FGameplayTag& /*Previous Input Tag*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FAbilityStatusChanged, const FGameplayTag& /*Ability Tag*/, const FGameplayTag& /*Status Tag*/);
UCLASS()
class PROJECT_FATEDBRAND_API UFatedBrandAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public :
	FAbilityEquipped AbilityEquipped;
	FAbilityStatusChanged AbilityStatusChanged;
	void AddCharacterActivateAbilities(const TArray<TSubclassOf<UFatedBrandGameplayAbility>>& ActivateAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UFatedBrandGameplayAbility>>& PassiveAbilities);
	bool bStartupAbilitiesGiven = false;

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	void OnAbilityInputPressed(const FGameplayTag& InInputTag);
	void OnAbilityInputReleased(const FGameplayTag& InInputTag);
	void OnAbilityInputHeld(const FGameplayTag& InInputTag);
	void ForEachAbility(const FForEachAbility& Delegate);

	void ClearInputTag(FGameplayAbilitySpec* Spec);
	void AssignInputTagToAbility(FGameplayAbilitySpec& Spec, const FGameplayTag& InputTag);
	bool InputTagIsEmpty(const FGameplayTag& InputTag);
	bool AbilityHasInputTag(const FGameplayAbilitySpec& Spec, const FGameplayTag& InputTag);
	bool AbilityHasAnyInputTag(const FGameplayAbilitySpec& Spec);
	bool IsPassiveAbility(const FGameplayAbilitySpec& Spec) const;


	FGameplayTag GetStatusFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayTag GetSlotFromAbilityTag(const FGameplayTag& AbilityTag);

	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayAbilitySpec* GetSpecWithNebulaSlot(const FGameplayTag& InputTag);

	UFUNCTION(BlueprintCallable, Category = "UpdateAbility")
	void UpdateAbilityStatuses(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable, Category = "UpdateAbility")
	void EquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag);
};