// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystem/FatedBrandAbilitySystemComponent.h"
#include "FatedBrandAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class PROJECT_FATEDBRAND_API UFatedBrandAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public :
    UFatedBrandAttributeSet();

    UPROPERTY(BlueprintReadOnly, Category = "Health")
    FGameplayAttributeData CurrentHealth;
    ATTRIBUTE_ACCESSORS(UFatedBrandAttributeSet, CurrentHealth)

    UPROPERTY(BlueprintReadOnly, Category = "MaxHealth")
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(UFatedBrandAttributeSet, MaxHealth)

    UPROPERTY(BlueprintReadOnly, Category = "Damage")
    FGameplayAttributeData AttackPower;
    ATTRIBUTE_ACCESSORS(UFatedBrandAttributeSet, AttackPower)
};
