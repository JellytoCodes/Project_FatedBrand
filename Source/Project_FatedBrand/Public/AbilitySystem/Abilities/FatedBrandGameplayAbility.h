// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FatedBrandStructTypes.h"
#include "Abilities/GameplayAbility.h"
#include "FatedBrandGameplayAbility.generated.h"

class AFatedBrandEnemy;
class UFatedBrandAbilitySystemComponent;
class AFatedBrandCharacter;
UCLASS()
class PROJECT_FATEDBRAND_API UFatedBrandGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public :
	UPROPERTY(EditAnywhere, Category = "Input")
	FGameplayTag StartupInputTag;

	UFUNCTION(BlueprintCallable)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr);

protected :
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintPure, Category = "FatedBrand|Ability")
	UFatedBrandAbilitySystemComponent* GetFatedBrandAbilitySystemComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "FatedBrand|Ability")
	AFatedBrandCharacter* GetFatedBrandCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "FatedBrand|Ability")
	AFatedBrandEnemy* GetFatedBrandEnemyFromActorInfo();

	FActiveGameplayEffectHandle NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle);

	UFUNCTION(BlueprintCallable, Category = "FatedBrand|Ability", meta = (DisplayName = "ApplyGameplayEffectSpecHandleToTargetActor"))
	FActiveGameplayEffectHandle BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag DamageType;

private :
	TWeakObjectPtr<AFatedBrandCharacter> CachedFatedBrandCharacter;
	TWeakObjectPtr<AFatedBrandEnemy> CachedFatedBrandEnemy;
};
