// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FatedBrandUserWidget.generated.h"

class UFatedBrandHotBarWidget;
class UFatedBrandNebulaSocketWidget;
class UObject;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSelectSocket);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSelectQuickSlot);

UCLASS()
class PROJECT_FATEDBRAND_API UFatedBrandUserWidget : public UUserWidget
{
	GENERATED_BODY()

public :
	UPROPERTY(BlueprintAssignable)
	FOnSelectSocket OnSelectSocketDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnSelectQuickSlot SelectQuickSlotDelegate;

	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	UFUNCTION(BlueprintCallable, Category = "NebulaSocket")
	void InitializeNebulaSocket(UObject* InWidgetController);

	UFUNCTION(BlueprintCallable, Category = "NebulaSocket")
	void CallSelectNebulaSocket(int32 PrevIndex, int32 CurrentIndex);

	UFUNCTION(BlueprintCallable, Category = "NebulaSocket")
	void CallSelectSocketFocusing(const int32 CurrentIndex, const bool IsSelectSocketFocusing);

	UFUNCTION(BlueprintCallable, Category = "QuickSlot")
	void InitializeQuickSlot(UObject* InWidgetController);

	UFUNCTION(BlueprintCallable, Category = "QuickSlot")
	void CallSelectQuickSlot(int32 PrevIndex, int32 CurrentIndex);

	UFUNCTION(BlueprintCallable, Category = "QuickSlot")
	void CallDeselectQuickSlot();

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected :
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();

private :
	TArray<TObjectPtr<UFatedBrandUserWidget>> SlotWidgets;

	TArray<TObjectPtr<UFatedBrandUserWidget>> QuickSlots;
};
