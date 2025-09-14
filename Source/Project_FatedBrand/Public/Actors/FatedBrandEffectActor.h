// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "FatedBrandEffectActor.generated.h"

class UGameplayEffect;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	RemoveOnEndOverlap,
	DoNotRemove
};

USTRUCT(BlueprintType)
struct FFatedBrandGameplayEffectParams
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowedClasses = "GameplayEffect"))
	TSubclassOf<UGameplayEffect> GameplayEffectClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEffectApplicationPolicy EffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEffectRemovalPolicy EffectRemovalPolicy = EEffectRemovalPolicy::DoNotRemove;
};

UCLASS()
class PROJECT_FATEDBRAND_API AFatedBrandEffectActor : public AActor
{
	GENERATED_BODY()

public:	
	virtual void Tick(float DeltaTime) override;

protected :
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, FFatedBrandGameplayEffectParams& InGameplayEffectParams);

	UFUNCTION(BlueprintCallable)
	void OnBeginOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Applied Effects")
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Applied Effects")
	bool bDestroyOnEffectApplication = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Applied Effects")
	bool bApplyEffectsToEnemies = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Applied Effects")
	TArray<FFatedBrandGameplayEffectParams> FatedBrandGameplayEffectParams;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Applied Effects")
	float ActorLevel = 1.f;
};
