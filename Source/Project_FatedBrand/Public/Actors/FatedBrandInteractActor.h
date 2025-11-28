// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ActorInteractInterface.h"
#include "FatedBrandInteractActor.generated.h"

class UNiagaraComponent;
class UFatedBrandAbilitySystemComponent;
class USphereComponent;
class UNiagaraSystem;

UCLASS()
class PROJECT_FATEDBRAND_API AFatedBrandInteractActor : public AActor, public IActorInteractInterface
{
	GENERATED_BODY()
	
public:	
	AFatedBrandInteractActor();

	virtual void GiveAbilityToTarget_Implementation(AActor* TargetActor) override;
	virtual FGameplayTag GetConditionTag_Implementation() override;
	virtual bool GetMatchesTagByTarget_Implementation(AActor* TargetActor) override;
	virtual void OnHit_Implementation() override;
	virtual AFatedBrandInteractActor* GetInteractActor_Implementation() override;

protected :
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

private :
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FGameplayTag ConditionTag;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FGameplayTag AbilityTag;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> Effect;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UNiagaraComponent> EffectComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UParticleSystem> ImpactParticle;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UParticleSystem> Particle;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UParticleSystemComponent> ParticleComponent;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USphereComponent> Sphere;


};
