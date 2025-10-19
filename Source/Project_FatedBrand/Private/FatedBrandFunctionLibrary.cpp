// Fill out your copyright notice in the Description page of Project Settings.


#include "FatedBrandFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GenericTeamAgentInterface.h"
#include "Characters/FatedBrandCharacter.h"
#include "Game/FatedBrandGameModeBase.h"
#include "HUD/FatedBrandHUD.h"
#include "HUD/WidgetController/FatedBrandWidgetController.h"
#include "Kismet/GameplayStatics.h"

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

UDataAsset_AbilityInfo* UFatedBrandFunctionLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const AFatedBrandGameModeBase* FatedBrandGameMode = Cast<AFatedBrandGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (FatedBrandGameMode == nullptr) return nullptr;

	return FatedBrandGameMode->AbilityInfo;
}

FGameplayEffectContextHandle UFatedBrandFunctionLibrary::ApplyDamageEffect(FDamageEffectParams DamageEffectParams)
{
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();

	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);

	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);
	SpecHandle.Data->SetSetByCallerMagnitude(DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);

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
