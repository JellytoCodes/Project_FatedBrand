// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UGameplayEffect;
class UFatedBrandGameplayAbility;
class UFatedBrandAbilitySystemComponent;

UENUM()
enum class EStartUpCharacterName : uint8
{
	PlayerCharacter,
	TutorialEnemy
};

USTRUCT(BlueprintType)
struct FFatedBrandStartUpAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EStartUpCharacterName StartUpCharacterName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> StartUpGameplayEffect;
};

UCLASS()
class PROJECT_FATEDBRAND_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()

public :
	virtual void InitializeGameplayEffect(UFatedBrandAbilitySystemComponent* InASCToGive,EStartUpCharacterName StartUpCharacterName, int32 ApplyLevel = 1);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StartUpData")
	TArray<FFatedBrandStartUpAttributeInfo> StartUpAttributeInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StartUp|Abilities", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UFatedBrandGameplayAbility>> StartUpOffensiveAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StartUp|Abilities", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UFatedBrandGameplayAbility>> StartUpPassiveAbilities;
};
