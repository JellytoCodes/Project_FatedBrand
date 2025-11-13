// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/FatedBrandProjectileAbility.h"

#include "AbilitySystemComponent.h"
#include "Actors/FatedBrandProjectileActor.h"
#include "Interfaces/CombatInterface.h"

void UFatedBrandProjectileAbility::SpawnProjectile(const FVector& ProjectileTargetLocation, const FName SocketName, bool bOverridePitch, float PitchOverride)
{
	const FVector SocketLocation = ICombatInterface::Execute_GetProjectileSpawnSocketLocation(GetAvatarActorFromActorInfo(), SocketName);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch) Rotation.Pitch = PitchOverride;

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());

	AFatedBrandProjectileActor* ProjectileActor = GetWorld()->SpawnActorDeferred<AFatedBrandProjectileActor>
	(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	ProjectileActor->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

	ProjectileActor->FinishSpawning(SpawnTransform);
}
