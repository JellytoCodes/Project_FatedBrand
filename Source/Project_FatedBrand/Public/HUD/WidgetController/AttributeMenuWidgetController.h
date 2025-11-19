// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "FatedBrandEnumTypes.h"
#include "HUD/WidgetController/FatedBrandWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

struct FGameplayAttribute;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSelectSocketSignature, int32, CurrentAxisX, int32, SelectSocketIndex);

USTRUCT(BlueprintType)
struct FAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AttributeValue;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAttributeInfo&, Info);

UCLASS(BlueprintType, Blueprintable)
class PROJECT_FATEDBRAND_API UAttributeMenuWidgetController : public UFatedBrandWidgetController
{
	GENERATED_BODY()

public :
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FAttributeInfoSignature AttributeInfoDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnSelectSocketSignature SelectSocketDelegate;

	UFUNCTION(BlueprintCallable)
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	void WidgetAxisControl(const int32 AxisX, const int32 AxisY);

private :
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute);

	int32 CachedAxisX;
	int32 CachedAxisY;

	FAttributeInfo Info;
};
