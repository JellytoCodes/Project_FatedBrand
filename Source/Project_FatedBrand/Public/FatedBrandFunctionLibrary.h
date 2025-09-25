// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FatedBrandFunctionLibrary.generated.h"

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
#pragma endregion
};
