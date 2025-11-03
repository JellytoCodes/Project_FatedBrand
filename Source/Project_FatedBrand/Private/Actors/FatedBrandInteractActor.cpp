// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/FatedBrandInteractActor.h"

#include "AbilitySystemBlueprintLibrary.h"
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

}

void AFatedBrandInteractActor::GiveAbilityToTarget_Implementation(AActor* TargetActor)
{
	if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(TargetActor))
	{
		IPlayerInterface::Execute_UpdateAbilities(TargetActor, AbilityTag);

		Destroy();
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

void AFatedBrandInteractActor::BeginPlay()
{
	Super::BeginPlay();

	if (Effect) UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, Effect, GetActorLocation(), FRotator::ZeroRotator, FVector(0.5f, 0.5f, 0.5f));
}

void AFatedBrandInteractActor::Destroyed()
{
	if (ImpactEffect) UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());

	Super::Destroyed();
}
