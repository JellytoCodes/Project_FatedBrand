// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/WidgetController/NebulaMenuWidgetController.h"

#include "DataAssets/DataAsset_AbilityInfo.h"
#include "AbilitySystem/FatedBrandAbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"

void UNebulaMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	
}

void UNebulaMenuWidgetController::BindCallbacksToDependencies()
{
	
}

void UNebulaMenuWidgetController::NebulaSelected(const FGameplayTag& AbilityTag)
{
	if (bWaitingForEquipSelection)
	{
		FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
		StopWaitForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}

	FGameplayTag AbilityStatus;

	bool bTagValid = AbilityTag.IsValid();
	bool bTagNone = AbilityTag.MatchesTag(FatedBrandGameplayTags::Abilities_None);
	FGameplayAbilitySpec* AbilitySpec = GetFatedBrandASC()->GetSpecFromAbilityTag(AbilityTag);

	const bool bSpecValid = AbilitySpec != nullptr;

	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = FatedBrandGameplayTags::Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = GetFatedBrandASC()->GetStatusFromSpec(*AbilitySpec);
	}

	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;

	bool bEnableEquip = false;
	ShouldEnableButtons(AbilityStatus, bEnableEquip);

	// TODO : ��ų ���� �޴� ��� �߰��ϱ�
	FString Description = "TEST";

	NebulaSelectedDelegate.Broadcast(bEnableEquip, Description);
}

void UNebulaMenuWidgetController::NebulaDeselect()
{
	if (bWaitingForEquipSelection)
	{
		FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
		StopWaitForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}

	SelectedAbility.Ability = FatedBrandGameplayTags::Abilities_None;
	SelectedAbility.Status = FatedBrandGameplayTags::Abilities_Status_Locked;

	NebulaSelectedDelegate.Broadcast(false, FString());
}

void UNebulaMenuWidgetController::EquipButtonPressed()
{
	const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;

	WaitForEquipDelegate.Broadcast(AbilityType);
	bWaitingForEquipSelection = true;

	const FGameplayTag SelectedStatus = GetFatedBrandASC()->GetStatusFromAbilityTag(SelectedAbility.Ability);
	if (SelectedStatus.MatchesTagExact(FatedBrandGameplayTags::Abilities_Status_Equipped))
	{
		SelectedSlot = GetFatedBrandASC()->GetSlotFromAbilityTag(SelectedAbility.Ability);
	}
}

void UNebulaMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
	
}

void UNebulaMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, bool& bShouldEnableEquipButton)
{
	bShouldEnableEquipButton = false;
	if (AbilityStatus.MatchesTagExact(FatedBrandGameplayTags::Abilities_Status_Equipped))
	{
		bShouldEnableEquipButton = true;
	}
	else if (AbilityStatus.MatchesTagExact(FatedBrandGameplayTags::Abilities_Status_Eligible))
	{
		// ���� ��ų ��ȭ ��� ���� �� ���
	}
	else if (AbilityStatus.MatchesTagExact(FatedBrandGameplayTags::Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButton = true;
	}
	else if (AbilityStatus.MatchesTagExact(FatedBrandGameplayTags::Abilities_Status_Locked))
	{
		// false ����
	}
}
