// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/FatedBrandAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	OnCurrentHealthChanged.Broadcast(GetFatedBrandAS()->GetCurrentHealth());
	OnMaxHealthChanged.Broadcast(GetFatedBrandAS()->GetMaxHealth());
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
}
