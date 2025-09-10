// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/DataAsset_StartUpDataBase.h"
#include "DataAsset_PlayerStartUpData.generated.h"

UCLASS()
class PROJECT_FATEDBRAND_API UDataAsset_PlayerStartUpData : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()

public :
	virtual void GiveToAbilitySystemComponent(UFatedBrandAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1) override;
};
