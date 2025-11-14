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
	FDamageEffectParams CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr);

protected :
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	void ReceiveDeactivate(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintPure, Category = "FatedBrand|Ability")
	UFatedBrandAbilitySystemComponent* GetFatedBrandAbilitySystemComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "FatedBrand|Ability")
	AFatedBrandCharacter* GetFatedBrandCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "FatedBrand|Ability")
	AFatedBrandEnemy* GetFatedBrandEnemyFromActorInfo();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	bool bIsRadialDamage = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float RadialDamageOuterRadius = 0.f;

private :
	TWeakObjectPtr<AFatedBrandCharacter> CachedFatedBrandCharacter;
	TWeakObjectPtr<AFatedBrandEnemy> CachedFatedBrandEnemy;
};
