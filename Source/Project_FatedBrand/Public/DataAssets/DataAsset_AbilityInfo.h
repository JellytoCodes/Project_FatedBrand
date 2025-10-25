// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "FatedBrandStructTypes.h"
#include "DataAsset_AbilityInfo.generated.h"

class UGameplayAbility;

UCLASS()
class PROJECT_FATEDBRAND_API UDataAsset_AbilityInfo : public UDataAsset
{
	GENERATED_BODY()

public :
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInformation")
	TArray<FFatedBrandAbilityInfo> AbilityInformation;

	FFatedBrandAbilityInfo FindAbilityInfoForTag(const FGameplayTag& AbilityTag) const;
};
