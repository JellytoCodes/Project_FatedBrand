// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DataAssets/DataAsset_StartUpDataBase.h"
#include "DataAsset_PlayerStartUpData.generated.h"

class UFatedBrandGameplayAbility;

USTRUCT(BlueprintType)
struct FFatedBrandAbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UFatedBrandGameplayAbility> AbilityToGrant;

	bool IsValid() const;
	
};

UCLASS()
class PROJECT_FATEDBRAND_API UDataAsset_PlayerStartUpData : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()

public :
	virtual void GiveToAbilitySystemComponent(UFatedBrandAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1) override;

private :
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData", meta = (TitleProperty = "InputTag"))
	TArray<FFatedBrandAbilitySet> CharacterStartUpAbilitySets;
};
