// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/FatedBrandHUD.h"

#include "HUD/Widgets/FatedBrandUserWidget.h"
#include "HUD/WidgetController/OverlayWidgetController.h"
#include "HUD/WidgetController/NebulaMenuWidgetController.h"
#include "HUD/Widgets/LoadScreenWidget.h"
#include "HUD/ViewModel/MVVM_LoadScreen.h"
#include "HUD/WidgetController/AttributeMenuWidgetController.h"
#include "HUD/WidgetController/PauseMenuWidgetController.h"
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

UPauseMenuWidgetController* AFatedBrandHUD::GetPauseMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (PauseWidgetController == nullptr)
	{
		PauseWidgetController = NewObject<UPauseMenuWidgetController>(this, PauseWidgetControllerClass);
		PauseWidgetController->SetWidgetControllerParams(WCParams);
		PauseWidgetController->BindCallbacksToDependencies();
	}
	return PauseWidgetController;
}

UAttributeMenuWidgetController* AFatedBrandHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}
	return AttributeMenuWidgetController;
}

void AFatedBrandHUD::InitOverlay(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UFatedBrandUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(OverlayWidgetController);

	Widget->AddToViewport();

	if (NebulaWidget == nullptr)
	{
		UUserWidget* LocalNebulaWidget = CreateWidget<UUserWidget>(GetWorld(), NebulaWidgetClass);
		NebulaWidget = Cast<UFatedBrandUserWidget>(LocalNebulaWidget);

		NebulaWidget->SetWidgetController(NebulaWidgetController);
		
		NebulaWidget->AddToViewport();
		NebulaWidget->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
		NebulaWidget->SetPositionInViewport(ViewportSize * 0.5f, true);
		NebulaWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (PauseWidget == nullptr)
	{
		UUserWidget* LocalPauseWidget = CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass);
		PauseWidget = Cast<UFatedBrandUserWidget>(LocalPauseWidget);

		PauseWidget->SetWidgetController(PauseWidgetController);

		PauseWidget->AddToViewport();
		PauseWidget->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
		PauseWidget->SetPositionInViewport(ViewportSize * 0.5f, true);
		PauseWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	WidgetController->BroadcastInitialValues();
}

void AFatedBrandHUD::HideOverlay()
{
	if (OverlayWidget == nullptr) return;
	OverlayWidget->SetVisibility(ESlateVisibility::Hidden);
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

void AFatedBrandHUD::VisiblePauseMenu(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	if (PauseWidget == nullptr) return;

	PauseWidget->SetVisibility(ESlateVisibility::Visible);
}

void AFatedBrandHUD::HidePauseMenu()
{
	if (PauseWidget == nullptr) return;

	PauseWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AFatedBrandHUD::CreateSaveMenuWidget()
{
	SaveScreenViewModel = NewObject<UMVVM_LoadScreen>(this, SaveScreenViewModelClass);
	SaveScreenViewModel->InitializeLoadSlots();

	SaveScreenWidget = CreateWidget<ULoadScreenWidget>(GetWorld(), SaveScreenWidgetClass);
	SaveScreenWidget->AddToViewport();
	SaveScreenWidget->BlueprintInitializeWidget();

	SaveScreenViewModel->LoadData();
}

void AFatedBrandHUD::CreateAttributeMenuWidget()
{
	if (AttributeWidget == nullptr)
	{
		AttributeWidget = CreateWidget<UFatedBrandUserWidget>(GetWorld(), AttributeWidgetClass);	
	}
	AttributeWidget->AddToViewport();
	AttributeWidget->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
	AttributeWidget->SetPositionInViewport(ViewportSize * 0.5f, true);
}

void AFatedBrandHUD::RemoveAttributeMenuWidget()
{
	if (AttributeWidget)
	{
		AttributeWidget->RemoveFromParent();
	}
}
