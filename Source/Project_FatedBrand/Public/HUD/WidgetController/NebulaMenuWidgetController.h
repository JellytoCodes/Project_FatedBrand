// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "FatedBrandGameplayTags.h"
#include "HUD/WidgetController/FatedBrandWidgetController.h"
#include "FatedBrandStructTypes.h"
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
	FGameplayTag InputTag = FGameplayTag();
};

class UFatedBrandUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNebulaSelectedSignature, bool, bEquipButtonEnabled, FString, DescriptionString);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSelectNebulaSocketSignature, int32, PrevIndex, int32, CurrentIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSelectHotBarSignature, int32, PrevIndex, int32, CurrentIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSocketFocusingSignature, int32, CurrentIndex, bool, IsSocketFocusing);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSelectSocketConfirmSignature);

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
	FOnSelectNebulaSocketSignature SelectNebulaSocketDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnSelectHotBarSignature SelectHotBarDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnSocketFocusingSignature SocketFocusingDelegate;

	UFUNCTION(BlueprintCallable)
	void NebulaSelected(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void OnAbilityEquip();

	UFUNCTION(BlueprintCallable)
	void HotBarSelected(const FGameplayTag& InputTag);

	void SetSelectSocketAxis(const int32 SocketX, const int32 SocketY);
	void SelectSocketFocusingController();

	void SelectSocketConfirm();

	UFUNCTION(BlueprintCallable)
	int32 GetSelectHotBarX() const { return SelectHotBarX; }

private :
	void ShouldEnableButtons(const FGameplayTag& AbilityStatus, bool&bShouldEnableEquipButton);
	FSelectedAbility SelectedAbility = {FatedBrandGameplayTags::Abilities_None, FatedBrandGameplayTags::Abilities_Status_Locked, FatedBrandGameplayTags::Input_Key_1};

	UFUNCTION()
	void UpdateEquipQuickSlot(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& InputTag, const FGameplayTag& PreviousInputTag);

	ENebulaSelectSocket NebulaSelectSocket;
	ENebulaSelectSocket NebulaHotBar;

	FGameplayTag SelectedSlot;
	int32 SelectSocketX = 0;
	int32 SelectSocketY = 0;
	int32 SelectHotBarX = 0;

	bool bIsSocketFocusing = false;

	int32 Wrap1(const int32 V, const int32 Min, const int32 Max);
};