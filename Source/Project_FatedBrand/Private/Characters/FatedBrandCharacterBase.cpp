// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/FatedBrandCharacterBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/FatedBrandAbilitySystemComponent.h"
#include "AbilitySystem/FatedBrandAttributeSet.h"
#include "Project_FatedBrand/Project_FatedBrand.h"

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

	if (Weapon)
	{
		Weapon->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnComponentBeginOverlap);	
	}
}

void AFatedBrandCharacterBase::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		Debug::Print(*OtherActor->GetActorNameOrLabel());
	}
}

void AFatedBrandCharacterBase::ToggleCurrentEquippedWeapon(const bool bShouldEnable)
{
	if (Weapon == nullptr) return;

	if (bShouldEnable)
	{
		Weapon->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}