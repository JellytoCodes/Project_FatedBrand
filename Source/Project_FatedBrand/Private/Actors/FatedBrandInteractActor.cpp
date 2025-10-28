// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/FatedBrandInteractActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "FatedBrandGameplayTags.h"
#include "Interfaces/CombatInterface.h"

AFatedBrandInteractActor::AFatedBrandInteractActor()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AFatedBrandInteractActor::GiveAbilityToTarget(AActor* TargetActor)
{
	if (TargetActor->ActorHasTag(FName("Enemy"))) return;

	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(TargetActor))
	{
		CombatInterface->UpdateAbilities(AbilityTag);
	}
}
