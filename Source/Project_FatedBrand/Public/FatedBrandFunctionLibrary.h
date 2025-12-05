// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FatedBrandStructTypes.h"
#include "FatedBrandFunctionLibrary.generated.h"

class UAttributeMenuWidgetController;
class UPauseMenuWidgetController;
class UFatedBrandSaveGame;
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

	UFUNCTION(BlueprintPure, Category = "FatedBrandFunctionBibrary", meta = (DefaultToSelf = "WorldContextObject"))
	static UPauseMenuWidgetController* GetPauseMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "FatedBrandFunctionBibrary", meta = (DefaultToSelf = "WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
#pragma endregion

#pragma region Ability System
	UFUNCTION(BlueprintCallable, Category = "FatedBrandFunctionBibrary")
	static UDataAsset_AbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "FatedBrandFunctionBibrary")
	static void InitializeDefaultAttributesFromSaveData(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, UFatedBrandSaveGame* SaveGame);

	UFUNCTION(BlueprintCallable, Category = "FatedBrandFunctionBibrary|Damage")
	static FGameplayEffectContextHandle ApplyDamageEffect(FDamageEffectParams DamageEffectParams);

	UFUNCTION(BlueprintCallable, Category = "FatedBrandFunctionBibrary|Hostile")
	static bool IsTargetPawnHostile(const APawn* QueryPawn, const APawn* TargetPawn);

	UFUNCTION(BlueprintCallable, Category = "FatedBrandFunctionBibrary|GameplayEffects")
	static void GetLivePlayersWithinCircle(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintCallable, Category = "FatedBrandFunctionBibrary|GameplayEffects")
	static void GetLivePlayersWithinLine(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, const FVector& Start, const FVector& End);

	UFUNCTION(BlueprintCallable, Category = "FatedBrandFunctionBibrary|Attributes")
	static void SetCurrentHealth(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, const float InHealth);


	UFUNCTION(BlueprintCallable, Category = "FatedBrandFunctionBibrary|GameplayCue")
	static void FatedBrandExecuteGameplayCue(const UAbilitySystemComponent* ASC, const FGameplayTag GameplayTagCue, const FGameplayCueParameters& GameplayCueParameters);
#pragma endregion
};
