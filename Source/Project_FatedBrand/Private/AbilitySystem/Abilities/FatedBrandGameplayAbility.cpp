// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/FatedBrandGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "FatedBrandFunctionLibrary.h"
#include "AbilitySystem/FatedBrandAbilitySystemComponent.h"
#include "Characters/FatedBrandCharacter.h"
#include "Characters/FatedBrandEnemy.h"

FDamageEffectParams UFatedBrandGameplayAbility::CauseDamage(AActor* TargetActor)
{
	const FDamageEffectParams CauseDamageParams = MakeDamageEffectParamsFromClassDefaults(TargetActor);

	UFatedBrandFunctionLibrary::ApplyDamageEffect(CauseDamageParams);

	return CauseDamageParams;
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

void UFatedBrandGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UFatedBrandAbilitySystemComponent* FatedBrandASC = Cast<UFatedBrandAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		FatedBrandASC->DeactivatePassiveAbility.AddUObject(this, &ThisClass::ReceiveDeactivate);
	}
}

void UFatedBrandGameplayAbility::ReceiveDeactivate(const FGameplayTag& AbilityTag)
{
	if (AbilityTags.HasTagExact(AbilityTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
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