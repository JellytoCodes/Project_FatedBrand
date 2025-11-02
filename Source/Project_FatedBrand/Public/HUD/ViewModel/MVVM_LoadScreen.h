// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"

class UMVVM_LoadSlot;
UCLASS()
class PROJECT_FATEDBRAND_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()

public :
	void InitializeLoadSlots();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;

	UFUNCTION(BlueprintPure)
	UMVVM_LoadSlot* GetLoadSlotViewModelByIndex(const int32 Index);

	UFUNCTION(BlueprintCallable)
	void SlotPressedForLoading(const int32 Slot);

	UFUNCTION(BlueprintCallable)
	void SlotPressedForSaving(const int32 Slot);

	UFUNCTION(BlueprintCallable)
	void SelectSlotPressedForPlay();

	UFUNCTION(BlueprintCallable)
	void SlotSelectedForWait(const int32 Slot);

	void LoadData();

private :
	UPROPERTY()
	TMap<int32, UMVVM_LoadSlot*> LoadSlots;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_0;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_1;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_2;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> SelectedSlot;
};


