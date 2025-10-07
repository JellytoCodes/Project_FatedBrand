// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/FatedBrandCharacterBase.h"

#include "AbilitySystem/FatedBrandAbilitySystemComponent.h"
#include "AbilitySystem/FatedBrandAttributeSet.h"

AFatedBrandCharacterBase::AFatedBrandCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FatedBrandAbilitySystemComponent = CreateDefaultSubobject<UFatedBrandAbilitySystemComponent>("FatedBrandAbilitySystemComponent");
	FatedBrandAttributeSet = CreateDefaultSubobject<UFatedBrandAttributeSet>("FatedBrandAttributeSet");
}

UAbilitySystemComponent* AFatedBrandCharacterBase::GetAbilitySystemComponent() const
{
	return GetFatedBrandAbilitySystemComponent();
}

void AFatedBrandCharacterBase::SendWeaponEquippedDelegate(const bool bIsEquip) const
{
	WeaponEquippedStatusDelegate.Broadcast(bIsEquip);
}

void AFatedBrandCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (FatedBrandAbilitySystemComponent)
	{
		FatedBrandAbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}
