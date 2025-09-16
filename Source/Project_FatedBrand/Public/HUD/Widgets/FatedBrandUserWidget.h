// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FatedBrandUserWidget.generated.h"

class UObject;

UCLASS()
class PROJECT_FATEDBRAND_API UFatedBrandUserWidget : public UUserWidget
{
	GENERATED_BODY()

public :
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected :
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
