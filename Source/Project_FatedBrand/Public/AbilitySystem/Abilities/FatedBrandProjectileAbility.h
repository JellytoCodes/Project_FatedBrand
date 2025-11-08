// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/FatedBrandGameplayAbility.h"
#include "FatedBrandProjectileAbility.generated.h"

class AFatedBrandProjectileActor;

UCLASS()
class PROJECT_FATEDBRAND_API UFatedBrandProjectileAbility : public UFatedBrandGameplayAbility
{
	GENERATED_BODY()

protected :
	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FName SocketName, bool bOverridePitch = false, float PitchOverride = 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AFatedBrandProjectileActor> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	int32 LoopSpawnProjectiles = 1;
};
