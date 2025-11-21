// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

class AFatedBrandPlayerState;

UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT_FATEDBRAND_API IPlayerInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SaveProgress(const FName& CheckPointTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateAbilities(const struct FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void MapBeginPlayMontage();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void MapEndPlayMontage(const TSoftObjectPtr<UWorld>& Level);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PlayerDie();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetVitalSurgeGage();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InteractSavePoint(const bool IsInteraction);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CanUpgradeAttribute(const bool IsInteraction);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AFatedBrandPlayerState* GetPlayerState();
};
