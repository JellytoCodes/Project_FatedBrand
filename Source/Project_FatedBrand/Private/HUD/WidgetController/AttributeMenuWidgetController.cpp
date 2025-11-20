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
	//GetFatedBrandASC()
}

void UAttributeMenuWidgetController::WidgetAxisControl(const int32 AxisX, const int32 AxisY)
{
    CachedAxisY = FMath::Clamp(CachedAxisY + AxisY, 0, 4);

	SelectSocketDelegate.Broadcast(AxisX, CachedAxisY);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute)
{
	Info.AttributeTag = AttributeTag;
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	
	AttributeInfoDelegate.Broadcast(Info);
}