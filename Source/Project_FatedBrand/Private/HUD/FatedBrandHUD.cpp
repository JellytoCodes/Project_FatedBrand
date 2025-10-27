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

	if (NebulaWidget == nullptr)
	{
		UUserWidget* LocalNebulaWidget = CreateWidget<UUserWidget>(GetWorld(), NebulaWidgetClass);
		NebulaWidget = Cast<UFatedBrandUserWidget>(LocalNebulaWidget);

		NebulaWidget->SetWidgetController(NebulaWidgetController);
		
		NebulaWidget->AddToViewport();
		NebulaWidget->SetPositionInViewport(NebulaWidgetPosition, true);
		WidgetController->BroadcastInitialValues();
		NebulaWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AFatedBrandHUD::VisibleNebulaMenu(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	if (NebulaWidget == nullptr) return;

	const FWidgetControllerParams WidgetControllerParams(PC, ASC, AS);
	UNebulaMenuWidgetController* WidgetController = GetNebulaMenuWidgetController(WidgetControllerParams);

	//포커싱 상태 강제 해제를 위해 true 설정
	WidgetController->bIsSocketFocusing = true;
	WidgetController->SelectSocketFocusingController();	
	NebulaWidget->SetVisibility(ESlateVisibility::Visible);
}

void AFatedBrandHUD::HideNebulaMenu()
{
	if (NebulaWidget == nullptr) return;
	
	NebulaWidget->SetVisibility(ESlateVisibility::Hidden);
	
}
