// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/FatedBrandInteractActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Interfaces/PlayerInterface.h"
#include "AbilitySystem/FatedBrandAbilitySystemComponent.h"

AFatedBrandInteractActor::AFatedBrandInteractActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(Sphere);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	EffectComponent = CreateDefaultSubobject<UNiagaraComponent>("EffectComponent");
	EffectComponent->SetupAttachment(Sphere);
	EffectComponent->bAutoActivate = false;

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>("ParticleComponent");
	ParticleComponent->SetupAttachment(Sphere);
	ParticleComponent->bAutoActivate = false;
}

void AFatedBrandInteractActor::GiveAbilityToTarget_Implementation(AActor* TargetActor)
{
	if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(TargetActor))
	{
		IPlayerInterface::Execute_UpdateAbilities(TargetActor, AbilityTag);
	}
}

FGameplayTag AFatedBrandInteractActor::GetConditionTag_Implementation()
{
	return ConditionTag;
}

bool AFatedBrandInteractActor::GetMatchesTagByTarget_Implementation(AActor* TargetActor)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	return TargetASC->HasMatchingGameplayTag(ConditionTag);
}

void AFatedBrandInteractActor::OnHit_Implementation()
{
	if (ImpactEffect)
	{
		EffectComponent->Deactivate();
		EffectComponent->SetAsset(ImpactEffect);
		EffectComponent->Activate();
	}

	if (ImpactParticle)
	{
		ParticleComponent->Deactivate();
		ParticleComponent->SetTemplate(ImpactParticle);
		ParticleComponent->Activate();
	}
}

AFatedBrandInteractActor* AFatedBrandInteractActor::GetInteractActor_Implementation()
{
	return this;
}

void AFatedBrandInteractActor::BeginPlay()
{
	Super::BeginPlay();

	if (Effect)
	{
		EffectComponent->SetAsset(Effect);
		EffectComponent->Activate();
	}

	if (Particle)
	{
		ParticleComponent->SetTemplate(Particle);
		ParticleComponent->Activate();
	}
}

void AFatedBrandInteractActor::Destroyed()
{

	Super::Destroyed();
}
