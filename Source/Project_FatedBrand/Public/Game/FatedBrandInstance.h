// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FatedBrandStructTypes.h"
#include "Engine/GameInstance.h"
#include "FatedBrandInstance.generated.h"

class UFatedBrandSaveGame;

UCLASS()
class PROJECT_FATEDBRAND_API UFatedBrandInstance : public UGameInstance
{
	GENERATED_BODY()

public :
	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void SetScalabilityLevel(const int32 Value);

	UPROPERTY()
	FName PlayerStartTag = FName();

	UPROPERTY()
	FString LoadSlotName = FString();

	UPROPERTY()
	int32 LoadSlotIndex = 0;

	UPROPERTY()
	TArray<FRunningCooldownData> RunningCooldowns;

	UPROPERTY()
	bool IsPlayerTravelTrigger = false;

	UPROPERTY()
	int32 ScalabilityLevel = 2;
};
