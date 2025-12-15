// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/WidgetController/AttributeMenuWidgetController.h"

#include "FatedBrandGameplayTags.h"
#include "AbilitySystem/FatedBrandAttributeSet.h"
#include "Controllers/FatedBrandPlayerController.h"

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

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag, const float AttributeValue, const float NewValue)
{
	if (AttributeTag == FatedBrandGameplayTags::Attributes_Locked || AttributeValue == NewValue) return;

	GetFatedBrandASC()->UpgradeAttribute(AttributeTag, NewValue);
}

void UAttributeMenuWidgetController::SpendEnhancedCore(const float SpendValue)
{
	GetFatedBrandASC()->SpendEnhancedCore(SpendValue);
	CachedSpendEnhancedCore = 0;
	CachedSpendEnhancedDelegate.Broadcast(CachedSpendEnhancedCore);
}

void UAttributeMenuWidgetController::CloseAttributeMenu()
{
	GetFatedBrandPC()->AttributeMenuDisable();
}

void UAttributeMenuWidgetController::WidgetAxisControl(const int32 AxisX, const int32 AxisY)
{
    CachedAxisY = FMath::Clamp(CachedAxisY + AxisY, 0, 4);

	SelectSocketDelegate.Broadcast(AxisX, CachedAxisY);
}

void UAttributeMenuWidgetController::SetCachedSpendEnhancedCore(const FGameplayTag AttributeTag, const float AttributeValue, const int UpgradeValue, const int CachedValue)
{
	const float InCachedValue = UpgradeValue < 0 ? CachedValue + 1 : CachedValue;
	
	const FName TagName(AttributeTag.ToString());

	if (const FRealCurve* Curve = SpendEnhancedCoreTable->FindCurve(TagName, TEXT("StatCurve")))
	{
		const float NewEnhancedCoreValue = Curve->Eval(InCachedValue);
		CachedSpendEnhancedCore = UpgradeValue > 0 ? CachedSpendEnhancedCore + NewEnhancedCoreValue : CachedSpendEnhancedCore - NewEnhancedCoreValue;
	}
	CachedSpendEnhancedDelegate.Broadcast(CachedSpendEnhancedCore);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute)
{
	Info.AttributeTag = AttributeTag;
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	
	AttributeInfoDelegate.Broadcast(Info);

	if (AttributeDescriptionTable)
	{
		const FName RowName(AttributeTag.ToString());
		if (const auto* Row = AttributeDescriptionTable->FindRow<FAttributeDescriptionRow>(RowName, TEXT("AttributeInfo")))
		{
			AttributeDescriptionDelegate.Broadcast(Row->AttributeName, Row->AttributeDescription);
		}
	}
}
