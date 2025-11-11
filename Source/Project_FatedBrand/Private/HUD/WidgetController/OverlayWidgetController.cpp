// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/WidgetController/OverlayWidgetController.h"

#include "FatedBrandGameplayTags.h"
#include "AbilitySystem/FatedBrandAttributeSet.h"
#include "DataAssets/DataAsset_AbilityInfo.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	OnCurrentHealthChanged.Broadcast(GetFatedBrandAS()->GetCurrentHealth());
	OnMaxHealthChanged.Broadcast(GetFatedBrandAS()->GetMaxHealth());
	OnVitalSurgeChanged.Broadcast(GetFatedBrandAS()->GetVitalSurge());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetFatedBrandAS()->GetCurrentHealthAttribute()).
	AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnCurrentHealthChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetFatedBrandAS()->GetMaxHealthAttribute()).
	AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnMaxHealthChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetFatedBrandAS()->GetVitalSurgeAttribute()).
	AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnVitalSurgeChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetFatedBrandAS()->GetEnhancedCoreAttribute()).
	AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnEnhancedCoreChanged.Broadcast(Data.NewValue);
	});

	if (GetFatedBrandASC())
	{
		GetFatedBrandASC()->AbilityEquipped.AddUObject(this, &ThisClass::OnAbilityEquipped);
		if (GetFatedBrandASC()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetFatedBrandASC()->AbilitiesGivenDelegate.AddUObject(this, &ThisClass::BroadcastAbilityInfo);
		}
	}
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const
{
	FFatedBrandAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = FatedBrandGameplayTags::Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = FatedBrandGameplayTags::Abilities_None;
	//Broadcast empty info if PreviousSlot is a valid slot, Only if equipping an already-equipped spell
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FFatedBrandAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);
}
