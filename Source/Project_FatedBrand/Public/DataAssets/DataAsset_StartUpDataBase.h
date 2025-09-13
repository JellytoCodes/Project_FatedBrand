// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UGameplayEffect;
class UFatedBrandGameplayAbility;
class UFatedBrandAbilitySystemComponent;

UCLASS()
class PROJECT_FATEDBRAND_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()

public :
	virtual void InitializeGameplayEffect(UFatedBrandAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StartUpData")
	TArray<TSubclassOf<UFatedBrandGameplayAbility>> ActivateOnGivenAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StartUpData")
	TArray<TSubclassOf<UFatedBrandGameplayAbility>> PassiveOnGivenAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StartUpData")
	TArray<TSubclassOf<UGameplayEffect>> StartUpGameplayEffects;
};
