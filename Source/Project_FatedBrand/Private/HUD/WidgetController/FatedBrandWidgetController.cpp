// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/WidgetController/FatedBrandWidgetController.h"

#include "AbilitySystem/FatedBrandAbilitySystemComponent.h"
#include "AbilitySystem/FatedBrandAttributeSet.h"
#include "Controllers/FatedBrandPlayerController.h"
#include "DataAssets/DataAsset_AbilityInfo.h"

void UFatedBrandWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UFatedBrandWidgetController::BroadcastAbilityInfo()
{
	if (!GetFatedBrandASC()->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FFatedBrandAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(GetFatedBrandASC()->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = GetFatedBrandASC()->GetInputTagFromSpec(AbilitySpec);
		Info.StatusTag = GetFatedBrandASC()->GetStatusFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
}

AFatedBrandPlayerController* UFatedBrandWidgetController::GetFatedBrandPC()
{
	if (FatedBrandPlayerController == nullptr)
	{
		FatedBrandPlayerController = Cast<AFatedBrandPlayerController>(PlayerController);
	}
	return FatedBrandPlayerController;
}

UFatedBrandAbilitySystemComponent* UFatedBrandWidgetController::GetFatedBrandASC()
{
	if (FatedBrandAbilitySystemComponent == nullptr)
	{
		FatedBrandAbilitySystemComponent = Cast<UFatedBrandAbilitySystemComponent>(AbilitySystemComponent);
	}
	return FatedBrandAbilitySystemComponent;
}

UFatedBrandAttributeSet* UFatedBrandWidgetController::GetFatedBrandAS()
{
	if (FatedBrandAttributeSet == nullptr)
	{
		FatedBrandAttributeSet = Cast<UFatedBrandAttributeSet>(AttributeSet);
	}
	return FatedBrandAttributeSet;
}
