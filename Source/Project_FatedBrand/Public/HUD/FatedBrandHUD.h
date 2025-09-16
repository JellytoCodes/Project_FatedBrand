// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FatedBrandHUD.generated.h"

class UFatedBrandUserWidget;
class UAttributeSet;
class UAbilitySystemComponent;
class UOverlayWidgetController;

struct FWidgetControllerParams;

UCLASS()
class PROJECT_FATEDBRAND_API AFatedBrandHUD : public AHUD
{
	GENERATED_BODY()

public :
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	void InitOverlay(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private :
	UPROPERTY()
	TObjectPtr<UFatedBrandUserWidget> OverlayWidget;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass")
	TSubclassOf<UFatedBrandUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
};
