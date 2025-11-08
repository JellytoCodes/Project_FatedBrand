// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/FatedBrandProjectileActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "FatedBrandFunctionLibrary.h"
#include "FatedBrandGameplayTags.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Project_FatedBrand/Project_FatedBrand.h"

AFatedBrandProjectileActor::AFatedBrandProjectileActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	//Sphere->SetCollisionObjectType()

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AFatedBrandProjectileActor::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeSpan);

	Sphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnSphereOverlap);
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

void AFatedBrandProjectileActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor)) return;
	if (!bHit) OnHit();

	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
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
	Destroy();
}



