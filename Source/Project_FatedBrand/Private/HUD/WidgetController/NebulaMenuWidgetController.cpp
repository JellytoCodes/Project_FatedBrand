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
	GetFatedBrandASC()->AbilityEquipped.AddUObject(this, &ThisClass::UpdateEquipQuickSlot);

	GetFatedBrandASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
	{
		if (AbilityInfo)
		{
			FFatedBrandAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});
}

void UNebulaMenuWidgetController::NebulaSelected(const FGameplayTag& AbilityTag)
{
	const FGameplayTag AbilityStatus = GetFatedBrandASC()->GetStatusFromAbilityTag(AbilityTag);
	FGameplayAbilitySpec* AbilitySpec = GetFatedBrandASC()->GetSpecFromAbilityTag(AbilityTag);

	bool bEnableEquip = false;
	ShouldEnableButtons(AbilityStatus, bEnableEquip);

	if (NebulaDescriptionTable)
	{
		const FGameplayTag NoneTag = FatedBrandGameplayTags::Abilities_None;
		const FName RowName(bEnableEquip ? AbilityTag.ToString() : NoneTag.ToString());
		if (const auto* Row = NebulaDescriptionTable->FindRow<FNebulaDescriptionRow>(RowName, TEXT("AbilityInfo")))
		{
			OnNebulaDescriptionDelegate.Broadcast(Row->NebulaName, Row->NebulaDescription);
		}
	}

	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;
}

void UNebulaMenuWidgetController::OnAbilityEquip()
{
	GetFatedBrandASC()->EquipAbility(SelectedAbility.Ability, SelectedAbility.InputTag);
}

void UNebulaMenuWidgetController::QuickSlotSelected(const FGameplayTag& InputTag)
{
	SelectedAbility.InputTag = InputTag;
}

void UNebulaMenuWidgetController::SetSelectSocketAxis(const int32 SocketX, const int32 SocketY)
{
	if (bIsSocketFocusing == false)
	{
		ENebulaSelectSocket PrevNebulaSelectSocket = NebulaSelectSocket;
	    SelectSocketX = Wrap1(SelectSocketX + SocketX, 0, 9);
	    SelectSocketY = Wrap1(SelectSocketY + SocketY, 0, 3);

	    const int32 index = (SelectSocketY * 10) + SelectSocketX;
		NebulaSelectSocket = static_cast<ENebulaSelectSocket>(index);

		SelectNebulaSocketDelegate.Broadcast(PrevNebulaSelectSocket, NebulaSelectSocket);
	}
    else
    {
		ENebulaSelectSocket PrevNebulaSelectHotBar = NebulaHotBar;
		SelectQuickSlotX = Wrap1(SelectQuickSlotX + SocketX, 0, 5);
		NebulaHotBar = static_cast<ENebulaSelectSocket>(SelectQuickSlotX);

	    SelectQuickSlotDelegate.Broadcast(PrevNebulaSelectHotBar, NebulaHotBar);
    }
}

void UNebulaMenuWidgetController::SelectSocketFocusingController()
{
	bIsSocketFocusing =! bIsSocketFocusing;
	SocketFocusingDelegate.Broadcast(NebulaSelectSocket, bIsSocketFocusing);
}

void UNebulaMenuWidgetController::SelectSocketConfirm()
{
	bIsSocketFocusing = false;
	OnAbilityEquip();
}

void UNebulaMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, bool& bShouldEnableEquipButton)
{
	bShouldEnableEquipButton = false;

	if (AbilityStatus.MatchesTagExact(FatedBrandGameplayTags::Abilities_Status_Equipped) || AbilityStatus.MatchesTagExact(FatedBrandGameplayTags::Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButton = true;
	}
	else if (AbilityStatus.MatchesTagExact(FatedBrandGameplayTags::Abilities_Status_Locked))
	{
		bShouldEnableEquipButton = false;
	}
}

void UNebulaMenuWidgetController::UpdateEquipQuickSlot(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag,const FGameplayTag& InputTag, const FGameplayTag& PreviousInputTag)
{
	FFatedBrandAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = FatedBrandGameplayTags::Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousInputTag;
	LastSlotInfo.AbilityTag = FatedBrandGameplayTags::Abilities_None;
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FFatedBrandAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = StatusTag;
	Info.InputTag = InputTag;

	AbilityInfoDelegate.Broadcast(Info);
}

int32 UNebulaMenuWidgetController::Wrap1(const int32 V, const int32 Min, const int32 Max)
{
    const int32 n = (Max - Min + 1);
    return Min + ( (V - Min) % n + n ) % n;
}
