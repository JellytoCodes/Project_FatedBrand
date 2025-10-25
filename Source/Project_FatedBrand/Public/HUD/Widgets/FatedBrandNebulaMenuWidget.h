// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/Widgets/FatedBrandUserWidget.h"
#include "FatedBrandStructTypes.h"
#include "FatedBrandNebulaMenuWidget.generated.h"

class UTextBlock;
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
	void CallSelectSocketFocusing(const int32 CurrentIndex, const bool IsSelectSocketFocusing);

	UFUNCTION(BlueprintCallable)
	void CallSelectHotBar(const int32 PrevIndex, const int32 CurrentIndex);

	UFUNCTION(BlueprintCallable)
	void SetNebulaDescription(FString InDescription);

	UFUNCTION(BlueprintCallable)
	void CallSelectSocketConfirm(FFatedBrandAbilityInfo CurrentInfo, const int32 CurrentIndex);

private :
	TArray<TObjectPtr<UFatedBrandNebulaSocketWidget>> SlotWidgets;

	TArray<TObjectPtr<UFatedBrandHotBarWidget>> HotBarWidgets;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TSoftObjectPtr<UTextBlock> NebulaDescription;
};
