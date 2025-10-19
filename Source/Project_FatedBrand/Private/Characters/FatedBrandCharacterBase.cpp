// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/FatedBrandCharacterBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "FatedBrandFunctionLibrary.h"
#include "AbilitySystem/FatedBrandAbilitySystemComponent.h"
#include "AbilitySystem/FatedBrandAttributeSet.h"
#include "Components/BoxComponent.h"
#include "Project_FatedBrand/Project_FatedBrand.h"

AFatedBrandCharacterBase::AFatedBrandCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Weapon->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnComponentBeginOverlap);
	Weapon->OnComponentHit.AddUniqueDynamic(this, &ThisClass::OnHit);
	
	LeftHandCollisionBox = CreateDefaultSubobject<UBoxComponent>("LeftHandCollisionBox");
	LeftHandCollisionBox->SetupAttachment(GetMesh());
	LeftHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//LeftHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnComponentBeginOverlap);
	LeftHandCollisionBox->OnComponentHit.AddUniqueDynamic(this, &ThisClass::OnHit);
	
	RightHandCollisionBox = CreateDefaultSubobject<UBoxComponent>("RightHandCollisionBox");
	RightHandCollisionBox->SetupAttachment(GetMesh());
	RightHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//RightHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnComponentBeginOverlap);
	RightHandCollisionBox->OnComponentHit.AddUniqueDynamic(this, &ThisClass::OnHit);

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

void AFatedBrandCharacterBase::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, LeftHandCollisionBoxAttachBoneName))
	{
		LeftHandCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, LeftHandCollisionBoxAttachBoneName);
	}
	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, RightHandCollisionBoxAttachBoneName))
	{
		RightHandCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, RightHandCollisionBoxAttachBoneName);
	}
}

void AFatedBrandCharacterBase::ToggleCurrentCollision(const bool bShouldEnable, const EToggleDamageType ToggleDamageType)
{
	const ECollisionEnabled::Type CurrentCollisionType = bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision;

	switch (ToggleDamageType)
	{
	case EToggleDamageType::EquippedWeapon :
		if (Weapon)
		{
			Weapon->SetCollisionEnabled(CurrentCollisionType);
		}
		break;

	case EToggleDamageType::LeftHand :
		LeftHandCollisionBox->SetCollisionEnabled(CurrentCollisionType);
		break;

	case EToggleDamageType::RightHand :
		RightHandCollisionBox->SetCollisionEnabled(CurrentCollisionType);
		break;
		
	default :
		break;
	}
}

void AFatedBrandCharacterBase::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) return;

	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		if (const APawn* HitPawn = Cast<APawn>(TargetASC->GetAvatarActor()))
		{
			if (UFatedBrandFunctionLibrary::IsTargetPawnHostile(this, HitPawn) == false) return;

			CombatDamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UFatedBrandFunctionLibrary::ApplyDamageEffect(CombatDamageEffectParams);
		}
	}
}

void AFatedBrandCharacterBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Debug::Print(OtherActor->GetActorNameOrLabel());
	if (OtherActor == this) return;

	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		if (const APawn* HitPawn = Cast<APawn>(TargetASC->GetAvatarActor()))
		{
			if (UFatedBrandFunctionLibrary::IsTargetPawnHostile(this, HitPawn) == false) return;

			CombatDamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UFatedBrandFunctionLibrary::ApplyDamageEffect(CombatDamageEffectParams);
		}
	}
}

void AFatedBrandCharacterBase::Die()
{
	if (bIsDeath) return;
	OnDeathDelegate.Broadcast(this);
	bIsDeath = true;
}