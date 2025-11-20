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

template <class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

UCLASS()
class PROJECT_FATEDBRAND_API UFatedBrandAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public :
    UFatedBrandAttributeSet();

    virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
    
    TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;

    UPROPERTY(BlueprintReadOnly, Category = "Strength")
    FGameplayAttributeData Strength;
    ATTRIBUTE_ACCESSORS(UFatedBrandAttributeSet, Strength)

    UPROPERTY(BlueprintReadOnly, Category = "Vigor")
    FGameplayAttributeData Vigor;
    ATTRIBUTE_ACCESSORS(UFatedBrandAttributeSet, Vigor)

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

    UPROPERTY(BlueprintReadOnly, Category = "RewardEnhancedCore")
    FGameplayAttributeData RewardEnhancedCore;
    ATTRIBUTE_ACCESSORS(UFatedBrandAttributeSet, RewardEnhancedCore)

private :
    void HandleIncomingDamage(FEffectProperties& Props);
    void HandleIncomingEnhancedCore(FEffectProperties& Props);
    void HandleIncomingVigor(FEffectProperties& Props);
    void HandleIncomingStrength(FEffectProperties& Props);

	void SetEffectProperties(const FGameplayEffectModCallbackData &Data, FEffectProperties& Props) const;
};
