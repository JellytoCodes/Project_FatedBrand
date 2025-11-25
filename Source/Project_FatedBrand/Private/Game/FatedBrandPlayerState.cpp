// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/FatedBrandPlayerState.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/FatedBrandAbilitySystemComponent.h"
#include "AbilitySystem/FatedBrandAttributeSet.h"
#include "Game/FatedBrandInstance.h"
#include "Project_FatedBrand/Project_FatedBrand.h"

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

void AFatedBrandPlayerState::CaptureRunningCooldowns()
{
	if (!AbilitySystemComponent) return;

	if (UFatedBrandInstance* FatedBrandInstance = Cast<UFatedBrandInstance>(GetGameInstance()))
	{
		FatedBrandInstance->RunningCooldowns.Empty();

		for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
		{
			const UGameplayAbility* AbilityCDO = Spec.Ability;
			if (!AbilityCDO || !Spec.GetDynamicSpecSourceTags().HasTag(FGameplayTag::RequestGameplayTag(FName("Input")))) continue;

			if (const UGameplayEffect* CooldownGE = AbilityCDO->GetCooldownGameplayEffect())
			{
				const FGameplayTagContainer CooldownTags = *AbilityCDO->GetCooldownTags();

				float TimeRemaining = 0.f;
				float CooldownDuration = 0.f;
				AbilityCDO->GetCooldownTimeRemainingAndDuration(
				Spec.Handle,
				AbilitySystemComponent->AbilityActorInfo.Get(),
				TimeRemaining,
				CooldownDuration);

				if (TimeRemaining > 0.f)
				{
					FRunningCooldownData Data;
					for (const FGameplayTag Tag : CooldownTags)
					{
						if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Cooldown")))) Data.CooldownTag = Tag;
					}
					Data.CooldownEffectClass = CooldownGE->GetClass();

					Data.RemainingTime = TimeRemaining;

					FatedBrandInstance->RunningCooldowns.Add(Data);
				}
			}
		}
	}
}

void AFatedBrandPlayerState::ApplyRunningCooldowns()
{
	if (!AbilitySystemComponent) return;

	if (UFatedBrandInstance* FatedBrandInstance = Cast<UFatedBrandInstance>(GetGameInstance()))
	{
		if (FatedBrandInstance->RunningCooldowns.Num() == 0) return;

		for (const FRunningCooldownData& Data : FatedBrandInstance->RunningCooldowns)
		{
			const FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
			FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Data.CooldownEffectClass, 1.f, Context);

			if (!SpecHandle.IsValid()) continue;

			FGameplayEffectSpec* Spec = SpecHandle.Data.Get();

			Spec->SetSetByCallerMagnitude(Data.CooldownTag, Data.RemainingTime);
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec);
		}
		FatedBrandInstance->RunningCooldowns.Empty();
	}
}