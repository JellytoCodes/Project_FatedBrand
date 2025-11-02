// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/FatedBrandGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "FatedBrandFunctionLibrary.h"
#include "FatedBrandGameplayTags.h"
#include "AbilitySystem/FatedBrandAbilitySystemComponent.h"
#include "Characters/FatedBrandCharacter.h"
#include "Characters/FatedBrandEnemy.h"

void UFatedBrandGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1);
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);


	float ScaledDamage = Damage;
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, ScaledDamage);

	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), TargetASC);

	const FGameplayTag HitReactTag = FatedBrandGameplayTags::Event_HitReact;
	TargetASC->TryActivateAbilitiesByTag(HitReactTag.GetSingleTagContainer());
}

FDamageEffectParams UFatedBrandGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor)
{
	FDamageEffectParams Params;

	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.DamageGameplayEffectClass = DamageEffectClass;
	Params.DamageType = DamageType;
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.BaseDamage = Damage;
	Params.AbilityLevel = GetAbilityLevel();
	return Params;
}

UFatedBrandAbilitySystemComponent* UFatedBrandGameplayAbility::GetFatedBrandAbilitySystemComponentFromActorInfo() const
{
	return Cast<UFatedBrandAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}

AFatedBrandCharacter* UFatedBrandGameplayAbility::GetFatedBrandCharacterFromActorInfo()
{
	if (!CachedFatedBrandCharacter.IsValid())
	{
		CachedFatedBrandCharacter = Cast<AFatedBrandCharacter>(CurrentActorInfo->AvatarActor);
	}
	return CachedFatedBrandCharacter.IsValid() ? CachedFatedBrandCharacter.Get() : nullptr;
}

AFatedBrandEnemy* UFatedBrandGameplayAbility::GetFatedBrandEnemyFromActorInfo()
{
	if (!CachedFatedBrandEnemy.IsValid())
	{
		CachedFatedBrandEnemy = Cast<AFatedBrandEnemy>(CurrentActorInfo->AvatarActor);
	}
	return CachedFatedBrandEnemy.IsValid() ? CachedFatedBrandEnemy.Get() : nullptr;
}