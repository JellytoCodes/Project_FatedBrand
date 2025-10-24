// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/WidgetController/NebulaMenuWidgetController.h"

#include "DataAssets/DataAsset_AbilityInfo.h"
#include "AbilitySystem/FatedBrandAbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "Project_FatedBrand/Project_FatedBrand.h"

void UNebulaMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	
}

void UNebulaMenuWidgetController::BindCallbacksToDependencies()
{
	
}

void UNebulaMenuWidgetController::NebulaSelected(const FGameplayTag& AbilityTag)
{
	FGameplayTag AbilityStatus;

	bool bTagValid = AbilityTag.IsValid();
	bool bTagNone = AbilityTag.MatchesTag(FatedBrandGameplayTags::Abilities_None);
	FGameplayAbilitySpec* AbilitySpec = GetFatedBrandASC()->GetSpecFromAbilityTag(AbilityTag);

	if (!bTagValid || bTagNone)
	{
		AbilityStatus = FatedBrandGameplayTags::Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = FatedBrandGameplayTags::Abilities_Status_Unlocked;
	}

	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;

	bool bEnableEquip = false;
	ShouldEnableButtons(AbilityStatus, bEnableEquip);

	// TODO : 스킬 정보 받는 기능 추가하기
	FString Description = "TEST";

	NebulaSelectedDelegate.Broadcast(bEnableEquip, Description);
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

	NebulaReassignedDelegate.Broadcast(AbilityTag);

	GetFatedBrandASC()->EquipAbility(AbilityTag, Slot);
}

void UNebulaMenuWidgetController::SetSelectSocketAxis(const int32 SocketX, const int32 SocketY)
{


	if (bIsSocketFocusing == false)
	{
		ENebulaSelectSocket PrevNebulaSelectSocket = NebulaSelectSocket;
	    SelectSocketX = Wrap1(SelectSocketX + SocketX, SOCKET_X_MIN - 1, SOCKET_X_MAX - 1);
	    SelectSocketY = Wrap1(SelectSocketY + SocketY, SOCKET_Y_MIN - 1, SOCKET_Y_MAX - 1);

	    const int32 index = (SelectSocketY * SOCKET_X_MAX) + SelectSocketX;
	    NebulaSelectSocket = static_cast<ENebulaSelectSocket>(index);

		OnSelectNebulaSocketDelegate.Broadcast(PrevNebulaSelectSocket, NebulaSelectSocket);	
	}
    else
    {
		ENebulaSelectSocket PrevNebulaSelectHotBar = NebulaHotBar;
		SelectHotBarX = Wrap1(SelectHotBarX + SocketX, HOTBAR_X_MIN - 1, HOTBAR_X_MAX - 1);
		NebulaHotBar = static_cast<ENebulaSelectSocket>(SelectHotBarX);

	    SelectHotBarDelegate.Broadcast(PrevNebulaSelectHotBar, NebulaHotBar);
    }
}

void UNebulaMenuWidgetController::SelectSocketFocusingController()
{
	SocketFocusingDelegate.Broadcast(NebulaSelectSocket);
	bIsSocketFocusing = true;
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

int32 UNebulaMenuWidgetController::Wrap1(const int32 V, const int32 Min, const int32 Max)
{
    const int32 n = (Max - Min + 1);
    return Min + ( (V - Min) % n + n ) % n;
}
