// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/LoadScreenHUD.h"

#include "Blueprint/UserWidget.h"
#include "HUD/ViewModel/MVVM_LoadScreen.h"
#include "HUD/Widgets/LoadScreenWidget.h"
#include "HUD/Widgets/FatedBrandUserWidget.h"

void ALoadScreenHUD::ShowMainMenu()
{
	if (MainMenuWidget == nullptr)
	{
		MainMenuWidget = CreateWidget<UFatedBrandUserWidget>(GetWorld(), MainMenuWidgetClass);

		MainMenuWidget->AddToViewport();
		MainMenuWidget->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
		//MainMenuWidget->SetPositionInViewport(ViewportSize * 0.5f, false);
	}
}

void ALoadScreenHUD::HideMainMenu()
{
	if (MainMenuWidget)
	{
		MainMenuWidget->RemoveFromParent();
		MainMenuWidget = nullptr;
	}
}

void ALoadScreenHUD::ShowLoadScreen()
{
	if (LoadScreenWidget == nullptr)
	{
		LoadScreenWidget = CreateWidget<ULoadScreenWidget>(GetWorld(), LoadScreenWidgetClass);
		LoadScreenWidget->BlueprintInitializeWidget();

		LoadScreenViewModel->LoadData();
		LoadScreenWidget->AddToViewport();
		LoadScreenWidget->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
		//LoadScreenWidget->SetPositionInViewport(ViewportSize * 0.5f, true);
	}
}

void ALoadScreenHUD::HideLoadScreen()
{
	if (LoadScreenWidget)
	{
		LoadScreenWidget->RemoveFromParent();
		LoadScreenWidget = nullptr;
	}
}

void ALoadScreenHUD::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	LoadScreenViewModel = NewObject<UMVVM_LoadScreen>(this, LoadScreenViewModelClass);
	LoadScreenViewModel->InitializeLoadSlots();
}
