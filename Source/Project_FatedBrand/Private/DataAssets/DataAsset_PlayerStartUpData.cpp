// Fill out your copyright notice in the Description page of Project Settings.

#include "DataAssets/DataAsset_PlayerStartUpData.h"

#include "AbilitySystem/FatedBrandAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/FatedBrandGameplayAbility.h"

void UDataAsset_PlayerStartUpData::InitializeGameplayEffect(UFatedBrandAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	Super::InitializeGameplayEffect(InASCToGive, ApplyLevel);
}
