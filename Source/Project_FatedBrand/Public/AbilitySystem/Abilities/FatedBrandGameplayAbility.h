// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FatedBrandGameplayAbility.generated.h"

class UFatedBrandAbilitySystemComponent;

UCLASS()
class PROJECT_FATEDBRAND_API UFatedBrandGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected :
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintPure, Category = "FatedBrand|Ability")
	UFatedBrandAbilitySystemComponent* GetFatedBrandAbilitySystemComponentFromActorInfo() const;

	FActiveGameplayEffectHandle NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle);

	UFUNCTION(BlueprintCallable, Category = "FatedBrand|Ability", meta = (DisplayName = "ApplyGameplayEffectSpecHandleToTargetActor"))
	FActiveGameplayEffectHandle BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle);
};
