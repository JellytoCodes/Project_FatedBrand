// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/WidgetController/FatedBrandWidgetController.h"
#include "OverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttribueChangedSignature, float, NewValue);

UCLASS()
class PROJECT_FATEDBRAND_API UOverlayWidgetController : public UFatedBrandWidgetController
{
	GENERATED_BODY()

public :
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttribueChangedSignature OnCurrentHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttribueChangedSignature OnMaxHealthChanged;


};
