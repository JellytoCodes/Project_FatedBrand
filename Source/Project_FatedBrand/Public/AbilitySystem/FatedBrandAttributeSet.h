// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "FatedBrandStructTypes.h"
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

    virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
    
    UPROPERTY(BlueprintReadOnly, Category = "Health")
    FGameplayAttributeData CurrentHealth;
    ATTRIBUTE_ACCESSORS(UFatedBrandAttributeSet, CurrentHealth)

    UPROPERTY(BlueprintReadOnly, Category = "MaxHealth")
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(UFatedBrandAttributeSet, MaxHealth)

    UPROPERTY(BlueprintReadOnly, Category = "VitalSurge")
    FGameplayAttributeData VitalSurge;
    ATTRIBUTE_ACCESSORS(UFatedBrandAttributeSet, VitalSurge)

    UPROPERTY(BlueprintReadOnly, Category = "Damage")
    FGameplayAttributeData AttackPower;
    ATTRIBUTE_ACCESSORS(UFatedBrandAttributeSet, AttackPower)

    UPROPERTY(BlueprintReadOnly, Category = "IncomingDamage")
    FGameplayAttributeData IncomingDamage;
    ATTRIBUTE_ACCESSORS(UFatedBrandAttributeSet, IncomingDamage)

    UPROPERTY(BlueprintReadOnly, Category = "EnhancedCore")
    FGameplayAttributeData EnhancedCore;
    ATTRIBUTE_ACCESSORS(UFatedBrandAttributeSet, EnhancedCore)

private :
    void HandleIncomingDamage(FEffectProperties& Props);

	void SetEffectProperties(const FGameplayEffectModCallbackData &Data, FEffectProperties& Props) const;
};
