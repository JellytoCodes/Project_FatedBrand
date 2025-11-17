// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/FatedBrandPlayerState.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/FatedBrandAbilitySystemComponent.h"
#include "AbilitySystem/FatedBrandAttributeSet.h"

AFatedBrandPlayerState::AFatedBrandPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UFatedBrandAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UFatedBrandAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AFatedBrandPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
