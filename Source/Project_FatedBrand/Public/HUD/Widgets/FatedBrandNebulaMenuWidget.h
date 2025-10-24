// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/Widgets/FatedBrandUserWidget.h"
#include "FatedBrandEnumTypes.h"
#include "FatedBrandNebulaMenuWidget.generated.h"

class UFatedBrandHotBarWidget;
class UFatedBrandNebulaSocketWidget;

UCLASS()
class PROJECT_FATEDBRAND_API UFatedBrandNebulaMenuWidget : public UFatedBrandUserWidget
{
	GENERATED_BODY()

public :
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void CallSelectSocket(int32 PrevIndex, int32 CurrentIndex);

	UFUNCTION(BlueprintCallable)
	void CallSelectSocketFocusing(int32 CurrentIndex);

	UFUNCTION(BlueprintCallable)
	void CallSelectHotBar(int32 PrevIndex, int32 CurrentIndex);

private :
	TArray<TObjectPtr<UFatedBrandNebulaSocketWidget>> SlotWidgets;

	TArray<TObjectPtr<UFatedBrandHotBarWidget>> HotBarWidgets;
};
