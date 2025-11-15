// Fill out your copyright notice in the Description page of Project Settings.


#include "FatedBrandFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "FatedBrandGameplayTags.h"
#include "GameplayCueManager.h"
#include "GenericTeamAgentInterface.h"
#include "AbilitySystem/FatedBrandAttributeSet.h"
#include "Characters/FatedBrandCharacter.h"
#include "Engine/OverlapResult.h"
#include "Game/FatedBrandGameModeBase.h"
#include "Game/FatedBrandSaveGame.h"
#include "HUD/FatedBrandHUD.h"
#include "HUD/WidgetController/FatedBrandWidgetController.h"
#include "Kismet/GameplayStatics.h"
#include "Project_FatedBrand/Project_FatedBrand.h"

bool UFatedBrandFunctionLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AFatedBrandHUD*& OutFatedBrandHUD)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutFatedBrandHUD = Cast<AFatedBrandHUD>(PC->GetHUD());
		if (OutFatedBrandHUD == nullptr) return false;
		
		if (const AFatedBrandCharacter* FatedBrandCharacter = Cast<AFatedBrandCharacter>(PC->GetPawn()))
		{
			OutWCParams.AttributeSet = FatedBrandCharacter->GetFatedBrandAttributeSet();
			OutWCParams.AbilitySystemComponent = FatedBrandCharacter->GetAbilitySystemComponent();
			OutWCParams.PlayerController = PC;

			return true;
		}
	}
	return false;
}

UOverlayWidgetController* UFatedBrandFunctionLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AFatedBrandHUD* FatedBrandHUD = nullptr;

	if (MakeWidgetControllerParams(WorldContextObject, WCParams, FatedBrandHUD))
	{
		return FatedBrandHUD->GetOverlayWidgetController(WCParams);
	}
	return nullptr;
}

UNebulaMenuWidgetController* UFatedBrandFunctionLibrary::GetNebulaMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AFatedBrandHUD* FatedBrandHUD = nullptr;

	if (MakeWidgetControllerParams(WorldContextObject, WCParams, FatedBrandHUD))
	{
		return FatedBrandHUD->GetNebulaMenuWidgetController(WCParams);
	}
	return nullptr;
}

UPauseMenuWidgetController* UFatedBrandFunctionLibrary::GetPauseMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AFatedBrandHUD* FatedBrandHUD = nullptr;

	if (MakeWidgetControllerParams(WorldContextObject, WCParams, FatedBrandHUD))
	{
		return FatedBrandHUD->GetPauseMenuWidgetController(WCParams);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UFatedBrandFunctionLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AFatedBrandHUD* FatedBrandHUD = nullptr;

	if (MakeWidgetControllerParams(WorldContextObject, WCParams, FatedBrandHUD))
	{
		return FatedBrandHUD->GetAttributeMenuWidgetController(WCParams);
	}
	return nullptr;
}

UDataAsset_AbilityInfo* UFatedBrandFunctionLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const AFatedBrandGameModeBase* FatedBrandGameMode = Cast<AFatedBrandGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (FatedBrandGameMode == nullptr) return nullptr;

	return FatedBrandGameMode->AbilityInfo;
}

void UFatedBrandFunctionLibrary::InitializeDefaultAttributesFromSaveData(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, UFatedBrandSaveGame* SaveGame)
{
	const UFatedBrandAttributeSet* AttrSet = ASC->GetSet<UFatedBrandAttributeSet>();
    if (!AttrSet) return;

	ASC->SetNumericAttributeBase(UFatedBrandAttributeSet::GetMaxHealthAttribute(), SaveGame->MaxHealth);
	ASC->SetNumericAttributeBase(UFatedBrandAttributeSet::GetCurrentHealthAttribute(), SaveGame->CurrentHealth);
	ASC->SetNumericAttributeBase(UFatedBrandAttributeSet::GetAttackPowerAttribute(), SaveGame->AttackPower);
	ASC->SetNumericAttributeBase(UFatedBrandAttributeSet::GetVitalSurgeAttribute(), SaveGame->VitalSurge);
}

FGameplayEffectContextHandle UFatedBrandFunctionLibrary::ApplyDamageEffect(FDamageEffectParams DamageEffectParams)
{
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();

	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);

	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);
	SpecHandle.Data->SetSetByCallerMagnitude(DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);

	const FGameplayTag HitReactTag = FatedBrandGameplayTags::Event_HitReact;
	DamageEffectParams.TargetAbilitySystemComponent->TryActivateAbilitiesByTag(HitReactTag.GetSingleTagContainer());

	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContextHandle;
}

bool UFatedBrandFunctionLibrary::IsTargetPawnHostile(const APawn* QueryPawn, const APawn* TargetPawn)
{
	check(QueryPawn && TargetPawn);

	const IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
	const IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

	if (QueryTeamAgent && TargetTeamAgent)
	{
		return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
	}
	return false;
}

void UFatedBrandFunctionLibrary::GetLivePlayersWithinCircle(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;

	SphereParams.AddIgnoredActors(ActorsToIgnore);
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;

		World->OverlapMultiByObjectType(
			Overlaps, 
			SphereOrigin, 
			FQuat::Identity,
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
			FCollisionShape::MakeSphere(Radius), 
			SphereParams);

		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

void UFatedBrandFunctionLibrary::GetLivePlayersWithinLine(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, const FVector& Start, const FVector& End)
{
	FCollisionQueryParams LineParams;

	LineParams.AddIgnoredActors(ActorsToIgnore);
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FHitResult> Overlaps;

		World->LineTraceMultiByObjectType(
			Overlaps,
			Start,
			End,
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
			LineParams);

		for (FHitResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

void UFatedBrandFunctionLibrary::SetCurrentHealth(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, const float InHealth)
{
	ASC->SetNumericAttributeBase(UFatedBrandAttributeSet::GetCurrentHealthAttribute(), InHealth);
}

void UFatedBrandFunctionLibrary::FatedBrandExecuteGameplayCue(const UAbilitySystemComponent* ASC, const FGameplayTag GameplayTagCue, const FGameplayCueParameters& GameplayCueParameters)
{
	if (!ASC || !GameplayTagCue.IsValid()) return;
	UGameplayCueManager::ExecuteGameplayCue_NonReplicated(ASC->GetAvatarActor(), GameplayTagCue, GameplayCueParameters);
}
