// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/FatedBrandInteractActor.h"

#include "Interfaces/PlayerInterface.h"

AFatedBrandInteractActor::AFatedBrandInteractActor()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AFatedBrandInteractActor::GiveAbilityToTarget(AActor* TargetActor)
{
	if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(TargetActor))
	{
		PlayerInterface->UpdateAbilities(AbilityTag);

		Destroy();
	}
}
