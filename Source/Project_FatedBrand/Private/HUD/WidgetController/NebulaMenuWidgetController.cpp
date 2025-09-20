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

	// TODO : 스킬 정보 받는 기능 추가하기
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
	bWaitingForEquipSelection = false;

	FFatedBrandAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = FatedBrandGameplayTags::Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = FatedBrandGameplayTags::Abilities_None;
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FFatedBrandAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);

	StopWaitForEquipDelegate.Broadcast(AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType);
	NebulaReassignedDelegate.Broadcast(AbilityTag);
	NebulaDeselect();
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
		// 추후 스킬 강화 기능 생길 시 사용
	}
	else if (AbilityStatus.MatchesTagExact(FatedBrandGameplayTags::Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButton = true;
	}
	else if (AbilityStatus.MatchesTagExact(FatedBrandGameplayTags::Abilities_Status_Locked))
	{
		// false 고정
	}
}
