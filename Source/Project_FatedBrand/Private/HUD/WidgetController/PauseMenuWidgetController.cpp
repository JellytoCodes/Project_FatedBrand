// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/WidgetController/PauseMenuWidgetController.h"

#include "Controllers/FatedBrandPlayerController.h"
#include "Game/FatedBrandGameModeBase.h"
#include "Game/FatedBrandInstance.h"
#include "Game/FatedBrandSaveGame.h"
#include "Kismet/GameplayStatics.h"

void UPauseMenuWidgetController::SetSelectMenu(int32 InAxisY)
{
	AxisY = FMath::Clamp(AxisY+InAxisY, 0, 3);

	SelectedMenuDelegate.Broadcast(AxisY);
}

void UPauseMenuWidgetController::EnteredInteraction()
{
	switch (AxisY)
	{
	case 0: // Resume
		Resume();
		break;

	case 1: // Retry
		Retry();
		break;

	case 2: // Main Menu
		MainMenu();
		break;
	case 3: // Quit Game
		QuitGame();
		break;

		default: // Do Not Activate
		break;
	}
}

void UPauseMenuWidgetController::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void UPauseMenuWidgetController::MainMenu()
{

}

void UPauseMenuWidgetController::Retry()
{
	if (AFatedBrandGameModeBase* GameModeBase = Cast<AFatedBrandGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		GameModeBase->RetryTravelToMap();
	}
}

void UPauseMenuWidgetController::Resume()
{
	GetFatedBrandPC()->PauseMenuDisable();
}
