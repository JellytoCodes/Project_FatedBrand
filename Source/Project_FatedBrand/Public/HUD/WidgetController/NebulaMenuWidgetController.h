// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "FatedBrandGameplayTags.h"
#include "HUD/WidgetController/FatedBrandWidgetController.h"
#include "NebulaMenuWidgetController.generated.h"

#define SOCKET_X_MIN 1
#define SOCKET_X_MAX 10
#define SOCKET_Y_MIN 1
#define SOCKET_Y_MAX 4

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNebulaSelectedSignature, bool, bEquipButtonEnabled, FString, DescriptionString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelectionSignature, const FGameplayTag&, AbilityType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNebulaReassignedSignature, const FGameplayTag&, AbilityTag);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSelectNebulaSocketSignature);

struct FSelectedAbility
{
	FGameplayTag Ability = FGameplayTag();
	FGameplayTag Status = FGameplayTag();
};

UENUM(BlueprintType)
enum ENebulaSelectSocket : uint8
{
	NebulaSocket1,
	NebulaSocket2,
	NebulaSocket3,
	NebulaSocket4,
	NebulaSocket5,
	NebulaSocket6,
	NebulaSocket7,
	NebulaSocket8,
	NebulaSocket9,
	NebulaSocket10,
	NebulaSocket11,
	NebulaSocket12,
	NebulaSocket13,
	NebulaSocket14,
	NebulaSocket15,
	NebulaSocket16,
	NebulaSocket17,
	NebulaSocket18,
	NebulaSocket19,
	NebulaSocket20,
	NebulaSocket21,
	NebulaSocket22,
	NebulaSocket23,
	NebulaSocket24,
	NebulaSocket25,
	NebulaSocket26,
	NebulaSocket27,
	NebulaSocket28,
	NebulaSocket29,
	NebulaSocket30,
	NebulaSocket31,
	NebulaSocket32,
	NebulaSocket33,
	NebulaSocket34,
	NebulaSocket35,
	NebulaSocket36,
	NebulaSocket37,
	NebulaSocket38,
	NebulaSocket39,
	NebulaSocket40
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
	FNebulaReassignedSignature NebulaReassignedDelegate;

	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature WaitForEquipDelegate;

	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature StopWaitForEquipDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnSelectNebulaSocketSignature OnSelectNebulaSocketDelegate;

	UFUNCTION(BlueprintCallable)
	void NebulaSelected(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void NebulaDeselect();

	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed();

	UFUNCTION(BlueprintCallable)
	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot);

	ENebulaSelectSocket NebulaSelectSocket;

	void SetSelectSocketAxis(const int32 SocketX, const int32 SocketY);

	UFUNCTION(BlueprintPure)
	ENebulaSelectSocket GetNebulaSelectSocket() const { return NebulaSelectSocket; }

private :
	void ShouldEnableButtons(const FGameplayTag& AbilityStatus, bool&bShouldEnableEquipButton);
	FSelectedAbility SelectedAbility = {FatedBrandGameplayTags::Abilities_None, FatedBrandGameplayTags::Abilities_Status_Locked};
	bool bWaitingForEquipSelection = false;

	FGameplayTag SelectedSlot;
	int32 SelectSocketX = 0;
	int32 SelectSocketY = 0;

	int32 Wrap1(const int32 V, const int32 Min, const int32 Max);
};