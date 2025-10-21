// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FatedBrandStructTypes.h"
#include "FatedBrandFunctionLibrary.generated.h"

struct FGameplayEffectContextHandle;
class AFatedBrandCharacterBase;
class AFatedBrandCharacter;
class UDataAsset_AbilityInfo;
class UNebulaMenuWidgetController;
class UOverlayWidgetController;
class AFatedBrandHUD;
struct FWidgetControllerParams;

UCLASS()
class PROJECT_FATEDBRAND_API UFatedBrandFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public :
#pragma region Widget Controller
	UFUNCTION(BlueprintPure, Category = "FatedBrandFunctionBibrary", meta = (DefaultToSelf = "WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AFatedBrandHUD*& OutFatedBrandHUD);

	UFUNCTION(BlueprintPure, Category = "FatedBrandFunctionBibrary", meta = (DefaultToSelf = "WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "FatedBrandFunctionBibrary", meta = (DefaultToSelf = "WorldContextObject"))
	static UNebulaMenuWidgetController* GetNebulaMenuWidgetController(const UObject* WorldContextObject);
#pragma endregion

#pragma region Ability System
	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	static UDataAsset_AbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem|Damage")
	static FGameplayEffectContextHandle ApplyDamageEffect(FDamageEffectParams DamageEffectParams);

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem|Hostile")
	static bool IsTargetPawnHostile(const APawn* QueryPawn, const APawn* TargetPawn);
#pragma endregion
};
