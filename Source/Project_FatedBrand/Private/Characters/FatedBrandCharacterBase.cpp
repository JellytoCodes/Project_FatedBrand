// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/FatedBrandCharacterBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "FatedBrandFunctionLibrary.h"
#include "FatedBrandGameplayTags.h"
#include "AbilitySystem/FatedBrandAbilitySystemComponent.h"
#include "AbilitySystem/FatedBrandAttributeSet.h"
#include "Components/BoxComponent.h"
#include "Interfaces/ActorInteractInterface.h"
#include "Project_FatedBrand/Project_FatedBrand.h"

AFatedBrandCharacterBase::AFatedBrandCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Weapon->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnComponentBeginOverlap);
	
	LeftHandCollisionBox = CreateDefaultSubobject<UBoxComponent>("LeftHandCollisionBox");
	LeftHandCollisionBox->SetupAttachment(GetMesh());
	LeftHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnComponentBeginOverlap);
	
	RightHandCollisionBox = CreateDefaultSubobject<UBoxComponent>("RightHandCollisionBox");
	RightHandCollisionBox->SetupAttachment(GetMesh());
	RightHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnComponentBeginOverlap);
	
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

	CurrentDamageType = ToggleDamageType;

	SetToggleCollisionEnabled(CurrentDamageType, CurrentCollisionType);
}

void AFatedBrandCharacterBase::SetToggleCollisionEnabled(const EToggleDamageType ToggleDamageType, const ECollisionEnabled::Type CurrentCollisionType) const
{
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

void AFatedBrandCharacterBase::OnHitTargetActor(AActor* HitActor)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);

	const bool bIsBlocking = TargetASC->HasMatchingGameplayTag(FatedBrandGameplayTags::Ability_Activate_Blocking);

	FGameplayEventData EventData;
	EventData.Instigator = CombatDamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	EventData.Target = TargetASC->GetAvatarActor();

	if (bIsBlocking)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetASC->GetAvatarActor(), FatedBrandGameplayTags::Event_SuccessfulBlock, EventData);
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(CombatDamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor(), FatedBrandGameplayTags::Event_SuccessfulBlock, EventData);
	}
	else
	{
		CombatDamageEffectParams.TargetAbilitySystemComponent = TargetASC;
		UFatedBrandFunctionLibrary::ApplyDamageEffect(CombatDamageEffectParams);
	}
}

void AFatedBrandCharacterBase::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) return;
	
	if (OtherActor->Implements<UCombatInterface>())
	{
		APawn* HitPawn = Cast<APawn>(OtherActor);
		if (UFatedBrandFunctionLibrary::IsTargetPawnHostile(this, HitPawn) == false) return;

		OnHitTargetActor(HitPawn);

		SetToggleCollisionEnabled(CurrentDamageType, ECollisionEnabled::NoCollision);
	}

	if (OtherActor->Implements<UActorInteractInterface>())
	{
		if (IActorInteractInterface* InteractInterface = Cast<IActorInteractInterface>(OtherActor))
		{
			if (GetFatedBrandAbilitySystemComponent()->HasMatchingGameplayTag(FatedBrandGameplayTags::Ability_Activate_BlastingZone))
			{
				InteractInterface->GiveAbilityToTarget(this);
			}
		}
		SetToggleCollisionEnabled(CurrentDamageType, ECollisionEnabled::NoCollision);
	}
}

void AFatedBrandCharacterBase::AddCharacterAbilities() const
{
	if (!StartUpData.IsNull())
	{
		if (UDataAsset_StartUpDataBase* LoadedData = StartUpData.LoadSynchronous())
		{
			LoadedData->InitializeGameplayEffect(FatedBrandAbilitySystemComponent, StartUpCharacterName, 1);

			// 캐릭터 기본 소유 액티브/패시브 스킬 ASC에 등록
			GetFatedBrandAbilitySystemComponent()->AddCharacterActivateAbilities(LoadedData->StartUpOffensiveAbilities);
			GetFatedBrandAbilitySystemComponent()->AddCharacterPassiveAbilities(LoadedData->StartUpPassiveAbilities);
		}
	}	
}

void AFatedBrandCharacterBase::Die()
{
	if (bIsDeath) return;
	OnDeathDelegate.Broadcast(this);
	bIsDeath = true;
}

AActor* AFatedBrandCharacterBase::GetAvatar_Implementation()
{
	return this;
}

bool AFatedBrandCharacterBase::IsDead_Implementation() const
{
	return bIsDeath;
}
