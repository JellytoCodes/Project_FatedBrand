// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/WidgetController/NebulaMenuWidgetController.h"

#include "DataAssets/DataAsset_AbilityInfo.h"
#include "AbilitySystem/FatedBrandAbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "Project_FatedBrand/Project_FatedBrand.h"

void UNebulaMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();

	NebulaSelectSocket = NebulaSocket1;
	SelectSocketX = 0;
	SelectSocketY = 0;
	
	NebulaHotBar = NebulaSocket1;
	SelectHotBarX = 0;

	bIsSocketFocusing = false;
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

	FString Description = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityDescription;

	NebulaSelectedDelegate.Broadcast(bEnableEquip, Description);
}

void UNebulaMenuWidgetController::OnAbilityEquip()
{
	bWaitingForEquipSelection = false;

	FFatedBrandAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability);
	Info.StatusTag = FatedBrandGameplayTags::Abilities_Status_Equipped;
	Info.InputTag = SelectedAbility.InputTag;
	AbilityInfoDelegate.Broadcast(Info);

	GetFatedBrandASC()->UpdateAbilityStatuses(Info);
}

void UNebulaMenuWidgetController::HotBarSelected(const FGameplayTag& InputTag)
{
	SelectedAbility.InputTag = InputTag;
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

		SelectNebulaSocketDelegate.Broadcast(PrevNebulaSelectSocket, NebulaSelectSocket);
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
	bIsSocketFocusing =! bIsSocketFocusing;
	SelectHotBarX = 0;
	SocketFocusingDelegate.Broadcast(NebulaHotBar, bIsSocketFocusing);
}

void UNebulaMenuWidgetController::SelectSocketConfirm()
{
	bIsSocketFocusing = false;
	SelectHotBarX = 0;
	OnAbilityEquip();
}

void UNebulaMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, bool& bShouldEnableEquipButton)
{
	if (AbilityStatus.MatchesTagExact(FatedBrandGameplayTags::Abilities_Status_Equipped) || AbilityStatus.MatchesTagExact(FatedBrandGameplayTags::Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButton = true;
	}
	else if (AbilityStatus.MatchesTagExact(FatedBrandGameplayTags::Abilities_Status_Locked))
	{
		bShouldEnableEquipButton = false;
	}
}

int32 UNebulaMenuWidgetController::Wrap1(const int32 V, const int32 Min, const int32 Max)
{
    const int32 n = (Max - Min + 1);
    return Min + ( (V - Min) % n + n ) % n;
}
