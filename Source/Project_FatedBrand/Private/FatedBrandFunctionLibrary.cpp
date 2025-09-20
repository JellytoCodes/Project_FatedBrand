// Fill out your copyright notice in the Description page of Project Settings.


#include "FatedBrandFunctionLibrary.h"

#include "Characters/FatedBrandCharacter.h"
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
