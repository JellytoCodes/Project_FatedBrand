// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "FatedBrandCharacterBase.generated.h"

class UDataAsset_StartUpDataBase;
class UFatedBrandAttributeSet;
class UFatedBrandAbilitySystemComponent;

UCLASS()
class PROJECT_FATEDBRAND_API AFatedBrandCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AFatedBrandCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	FORCEINLINE UFatedBrandAbilitySystemComponent* GetFatedBrandAbilitySystemComponent() const { return FatedBrandAbilitySystemComponent; }
	FORCEINLINE UFatedBrandAttributeSet* GetFatedBrandAttributeSet() const { return FatedBrandAttributeSet; }

protected:
	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	TObjectPtr<UFatedBrandAbilitySystemComponent> FatedBrandAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	TObjectPtr<UFatedBrandAttributeSet> FatedBrandAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	TSoftObjectPtr<UDataAsset_StartUpDataBase> StartUpData;

	virtual void InitAbilityActorInfo() { }
};
