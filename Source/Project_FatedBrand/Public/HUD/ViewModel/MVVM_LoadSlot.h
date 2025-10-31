// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Game/FatedBrandSaveGame.h"
#include "MVVM_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnableSelectSlot, bool, bEnable);

UCLASS()
class PROJECT_FATEDBRAND_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public :
	UPROPERTY(BlueprintAssignable)
	FEnableSelectSlot EnableSelectSlotDelegate;

	UPROPERTY()
	int32 SlotIndex;

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus;

	UPROPERTY()
	FName PlayerStartTag;

	UPROPERTY()
	FString MapAssetName;

	void SetLoadSlotName(FString InLoadSlotName);
	void SetMapName(FString InMapName);

	FString GetLoadSlotName() const { return LoadSlotName; }
	FString GetMapName() const { return MapName; }

private :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FString LoadSlotName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FString MapName;

};
