// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/FatedBrandHUD.h"

#include "HUD/Widgets/FatedBrandUserWidget.h"
#include "HUD/WidgetController/OverlayWidgetController.h"
#include "HUD/WidgetController/NebulaMenuWidgetController.h"

UOverlayWidgetController* AFatedBrandHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

UNebulaMenuWidgetController* AFatedBrandHUD::GetNebulaMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (NebulaWidgetController == nullptr)
	{
		NebulaWidgetController = NewObject<UNebulaMenuWidgetController>(this, NebulaWidgetControllerClass);
		NebulaWidgetController->SetWidgetControllerParams(WCParams);
		NebulaWidgetController->BindCallbacksToDependencies();
	}
	return NebulaWidgetController;
}

void AFatedBrandHUD::InitOverlay(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UFatedBrandUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(OverlayWidgetController);
	WidgetController->BroadcastInitialValues();

	Widget->AddToViewport();
}
