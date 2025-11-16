// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/FatedBrandAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "FatedBrandGameplayTags.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "Interfaces/CombatInterface.h"
#include "Project_FatedBrand/Project_FatedBrand.h"

UFatedBrandAttributeSet::UFatedBrandAttributeSet()
{
	InitVitalSurge(0.f);
	InitCurrentHealth(1.f);
	InitMaxHealth(1.f);
	InitAttackPower(1.f);
	InitEnhancedCore(0.f);
	InitRewardEnhancedCore(0.f);

	TagsToAttributes.Add(FatedBrandGameplayTags::Attributes_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(FatedBrandGameplayTags::Attributes_CurrentHealth, GetCurrentHealthAttribute);
	TagsToAttributes.Add(FatedBrandGameplayTags::Attributes_AttackPower, GetAttackPowerAttribute);
	TagsToAttributes.Add(FatedBrandGameplayTags::Attributes_EnhancedCore, GetEnhancedCoreAttribute);
}

void UFatedBrandAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Props);
	}

	if (Data.EvaluatedData.Attribute == GetVitalSurgeAttribute())
	{
		SetVitalSurge(FMath::Clamp(GetVitalSurge(),0.f, 100.f));
	}

	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		SetCurrentHealth(FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth()));	
	}

	if (Data.EvaluatedData.Attribute == GetEnhancedCoreAttribute())
	{
		SetEnhancedCore(GetEnhancedCore());
	}
}

void UFatedBrandAttributeSet::HandleIncomingDamage(FEffectProperties& Props)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.f);

	if (LocalIncomingDamage > 0.f)
	{
		const float NewHealth = GetCurrentHealth() - LocalIncomingDamage;
		SetCurrentHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

		if (NewHealth <= 0.f)
		{
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor))
			{
				CombatInterface->Die();
			}
			HandleIncomingEnhancedCore(Props);
		}
	}
}

void UFatedBrandAttributeSet::HandleIncomingEnhancedCore(FEffectProperties& Props)
{
	if (Props.SourceASC)
	{
		const float Reward = GetRewardEnhancedCore();
		const FGameplayAttribute EnhancedCoreAttr = UFatedBrandAttributeSet::GetEnhancedCoreAttribute();
		const float CurrentCore = Props.SourceASC->GetNumericAttribute(EnhancedCoreAttr);

		Props.SourceASC->SetNumericAttributeBase(EnhancedCoreAttr, CurrentCore + Reward);

		SetRewardEnhancedCore(0.f);
	}
}

void UFatedBrandAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();

		if (!Props.SourceController && Props.SourceAvatarActor)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}

		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);

		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}
