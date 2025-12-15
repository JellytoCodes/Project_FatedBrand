// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/FatedBrandProjectileActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "FatedBrandFunctionLibrary.h"
#include "FatedBrandGameplayTags.h"
#include "GameFramework/ProjectileMovementComponent.h"

AFatedBrandProjectileActor::AFatedBrandProjectileActor()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AFatedBrandProjectileActor::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeSpan);
}

void AFatedBrandProjectileActor::Destroyed()
{
	Super::Destroyed();
}

bool AFatedBrandProjectileActor::IsValidOverlap(AActor* OtherActor) const
{
	if (DamageEffectParams.SourceAbilitySystemComponent == nullptr) return false;
	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if (SourceAvatarActor == OtherActor) return false;
	if (OtherActor == this || OtherActor->IsA(AFatedBrandProjectileActor::StaticClass())) return false;
	if (UFatedBrandFunctionLibrary::IsTargetPawnHostile(Cast<APawn>(SourceAvatarActor), Cast<APawn>(OtherActor)) == false) return false;

	return true;
}

void AFatedBrandProjectileActor::OnHit()
{
	bHit = true;
}

void AFatedBrandProjectileActor::ApplyEffectToTarget(AActor* TargetActor)
{
	if (!IsValidOverlap(TargetActor)) return;
	if (!bHit) OnHit();

	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		const bool bIsBlocking = TargetASC->HasMatchingGameplayTag(FatedBrandGameplayTags::Ability_Activate_Blocking);

		FGameplayEventData EventData;
		EventData.Instigator = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
		EventData.Target = TargetASC->GetAvatarActor();

		if (bIsBlocking)
		{
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetASC->GetAvatarActor(), FatedBrandGameplayTags::Event_SuccessfulBlock, EventData);
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor(), FatedBrandGameplayTags::Event_SuccessfulBlock, EventData);
		}
		else
		{
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UFatedBrandFunctionLibrary::ApplyDamageEffect(DamageEffectParams);
		}
	}
	if (bDestroyOnEffectApplication) Destroy();
}