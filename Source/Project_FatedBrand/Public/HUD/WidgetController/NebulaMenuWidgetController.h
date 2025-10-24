// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "FatedBrandGameplayTags.h"
#include "HUD/WidgetController/FatedBrandWidgetController.h"
#include "FatedBrandEnumTypes.h"
#include "NebulaMenuWidgetController.generated.h"

#define SOCKET_X_MIN 1
#define SOCKET_X_MAX 10
#define SOCKET_Y_MIN 1
#define SOCKET_Y_MAX 4

#define HOTBAR_X_MIN 1
#define HOTBAR_X_MAX 6

struct FSelectedAbility
{
	FGameplayTag Ability = FGameplayTag();
	FGameplayTag Status = FGameplayTag();
};

class UFatedBrandUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNebulaSelectedSignature, bool, bEquipButtonEnabled, FString, DescriptionString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelectionSignature, const FGameplayTag&, AbilityType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNebulaReassignedSignature, const FGameplayTag&, AbilityTag);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSelectNebulaSocketSignature, int32, PrevIndex, int32, CurrentIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSelectHotBarSignature, int32, PrevIndex, int32, CurrentIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSocketFocusingSignature, int32, CurrentIndex);

UCLASS(BlueprintType, Blueprintable)
class PROJECT_FATEDBRAND_API UNebulaMenuWidgetController : public UFatedBrandWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FNebulaSelectedSignature NebulaSelectedDelegate;

	UPROPERTY(BlueprintAssignable)
	FNebulaReassignedSignature NebulaReassignedDelegate;

	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature WaitForEquipDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnSelectNebulaSocketSignature OnSelectNebulaSocketDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnSelectHotBarSignature SelectHotBarDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnSocketFocusingSignature SocketFocusingDelegate;

	UFUNCTION(BlueprintCallable)
	void NebulaSelected(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed();

	UFUNCTION(BlueprintCallable)
	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot);

	void SetSelectSocketAxis(const int32 SocketX, const int32 SocketY);
	void SelectSocketFocusingController();

	UFUNCTION(BlueprintCallable)
	void SetIsSocketFocusing(bool InSocketFocusing) { bIsSocketFocusing = InSocketFocusing; }

private :
	void ShouldEnableButtons(const FGameplayTag& AbilityStatus, bool&bShouldEnableEquipButton);
	FSelectedAbility SelectedAbility = {FatedBrandGameplayTags::Abilities_None, FatedBrandGameplayTags::Abilities_Status_Locked};
	bool bWaitingForEquipSelection = false;

	ENebulaSelectSocket NebulaSelectSocket;
	ENebulaSelectSocket NebulaHotBar;

	FGameplayTag SelectedSlot;
	int32 SelectSocketX = 0;
	int32 SelectSocketY = 0;
	int32 SelectHotBarX = 0;

	bool bIsSocketFocusing = false;

	int32 Wrap1(const int32 V, const int32 Min, const int32 Max);
};