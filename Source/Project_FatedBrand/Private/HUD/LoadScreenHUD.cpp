// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/LoadScreenHUD.h"

#include "Blueprint/UserWidget.h"
#include "HUD/ViewModel/MVVM_LoadScreen.h"
#include "HUD/Widgets/LoadScreenWidget.h"

void ALoadScreenHUD::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	LoadScreenViewModel = NewObject<UMVVM_LoadScreen>(this, LoadScreenViewModelClass);
	LoadScreenViewModel->InitializeLoadSlots();

	LoadScreenWidget = CreateWidget<ULoadScreenWidget>(GetWorld(), LoadScreenWidgetClass);
	LoadScreenWidget->AddToViewport();
	LoadScreenWidget->BlueprintInitializeWidget();

	LoadScreenWidget->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
	LoadScreenWidget->SetPositionInViewport(ViewportSize * 0.5f, true);

	LoadScreenViewModel->LoadData();
}
