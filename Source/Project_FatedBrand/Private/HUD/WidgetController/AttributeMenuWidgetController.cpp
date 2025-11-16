// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/FatedBrandAttributeSet.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	for (auto& Pair : GetFatedBrandAS()->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	for (auto& Pair : GetFatedBrandAS()->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).
		AddLambda([this, Pair](const FOnAttributeChangeData& Data)
		{
			BroadcastAttributeInfo(Pair.Key, Pair.Value());
		});
	}
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{

}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute)
{
	Info.AttributeTag = AttributeTag;
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
