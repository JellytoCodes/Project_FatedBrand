// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "FatedBrandGameplayTags.h"
#include "HUD/WidgetController/FatedBrandWidgetController.h"
#include "NebulaMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNebulaSelectedSignature, bool, bEquipButtonEnabled, FString, DescriptionString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelectionSignature, const FGameplayTag&, AbilityType);

struct FSelectedAbility
{
	FGameplayTag Ability = FGameplayTag();
	FGameplayTag Status = FGameplayTag();
};

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
	FWaitForEquipSelectionSignature WaitForEquipDelegate;

	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature StopWaitForEquipDelegate;

	UFUNCTION(BlueprintCallable)
	void NebulaSelected(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void NebulaDeselect();

	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed();

	UFUNCTION(BlueprintCallable)
	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot);

private :
	void ShouldEnableButtons(const FGameplayTag& AbilityStatus, bool&bShouldEnableEquipButton);
	FSelectedAbility SelectedAbility = {FatedBrandGameplayTags::Abilities_None, FatedBrandGameplayTags::Abilities_Status_Locked};
	bool bWaitingForEquipSelection = false;

	FGameplayTag SelectedSlot;
};
