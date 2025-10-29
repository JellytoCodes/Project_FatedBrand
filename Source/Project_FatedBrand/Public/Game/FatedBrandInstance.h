// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FatedBrandInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_FATEDBRAND_API UFatedBrandInstance : public UGameInstance
{
	GENERATED_BODY()

public :
	UPROPERTY()
	FName PlayerStartTag = FName();

	UPROPERTY()
	FString LoadSlotName = FString();

	UPROPERTY()
	int32 LoadSlotIndex = 0;
};
