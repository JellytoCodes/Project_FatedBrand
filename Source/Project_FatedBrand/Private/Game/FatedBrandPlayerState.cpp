// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/FatedBrandPlayerState.h"

#include "AbilitySystemComponent.h"
#include "FatedBrandGameplayTags.h"
#include "AbilitySystem/FatedBrandAbilitySystemComponent.h"
#include "AbilitySystem/FatedBrandAttributeSet.h"
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

	RunningCooldowns.Empty();

	if (const FGameplayAbilityActorInfo* ActorInfo = AbilitySystemComponent->AbilityActorInfo.Get())
	{
		for (const auto& Spec : AbilitySystemComponent->GetActivatableAbilities())
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
				ActorInfo,
				TimeRemaining,
				CooldownDuration);

				if (TimeRemaining > 0.f)
				{
					FAbilityCooldownSaveData Data;
					for (const FGameplayTag Tag : CooldownTags)
					{
						Debug::Print(Tag.ToString(), Spec.Ability->GetCooldownTimeRemaining());
					}

					Data.CooldownRemaining = TimeRemaining;
					Data.CooldownDuration = CooldownDuration;
				}

				Debug::Print(Spec.Ability->GetCooldownGameplayEffect()->GetName());

			}
		}
	}
}
