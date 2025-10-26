// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/FatedBrandHUD.h"

#include "HUD/Widgets/FatedBrandUserWidget.h"
#include "HUD/WidgetController/OverlayWidgetController.h"
#include "HUD/WidgetController/NebulaMenuWidgetController.h"
#include "Project_FatedBrand/Project_FatedBrand.h"

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

void AFatedBrandHUD::CreateNebulaMenu(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	const FWidgetControllerParams WidgetControllerParams(PC, ASC, AS);
	UNebulaMenuWidgetController* WidgetController = GetNebulaMenuWidgetController(WidgetControllerParams);

	if (NebulaWidget == nullptr)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), NebulaWidgetClass);
		NebulaWidget = Cast<UFatedBrandUserWidget>(Widget);

		NebulaWidget->SetWidgetController(NebulaWidgetController);
		
		NebulaWidget->AddToViewport();
		NebulaWidget->SetPositionInViewport(FVector2D(400, 100), true);
	}

	WidgetController->BroadcastInitialValues();
	NebulaWidget->SetVisibility(ESlateVisibility::Visible);
}

void AFatedBrandHUD::RemoveNebulaMenu()
{
	if (NebulaWidget)
	{
		NebulaWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
