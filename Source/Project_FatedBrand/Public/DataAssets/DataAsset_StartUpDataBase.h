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
	virtual void GiveToAbilitySystemComponent(UFatedBrandAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);

protected :
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StartUpData")
	TArray<TSubclassOf<UFatedBrandGameplayAbility>> ActivateOnGivenAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StartUpData")
	TArray<TSubclassOf<UFatedBrandGameplayAbility>> ReactiveAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StartUpData")
	TArray<TSubclassOf<UGameplayEffect>> StartUpGameplayEffects;

	void GrantAbilities(const TArray<TSubclassOf<UFatedBrandGameplayAbility>>& InAbilitiesToGive, UFatedBrandAbilitySystemComponent* InASCToGive, int32 ApplyLevel);
};
