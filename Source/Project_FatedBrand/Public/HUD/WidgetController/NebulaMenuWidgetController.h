// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/WidgetController/FatedBrandWidgetController.h"
#include "NebulaMenuWidgetController.generated.h"

UCLASS(BlueprintType, Blueprintable)
class PROJECT_FATEDBRAND_API UNebulaMenuWidgetController : public UFatedBrandWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
};
