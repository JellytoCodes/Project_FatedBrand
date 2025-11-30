// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LoadScreenHUD.generated.h"

class UFatedBrandUserWidget;
class UMVVM_LoadScreen;
class ULoadScreenWidget;

UCLASS()
class PROJECT_FATEDBRAND_API ALoadScreenHUD : public AHUD
{
	GENERATED_BODY()

public :
#pragma region LoadScreen
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULoadScreenWidget> LoadScreenWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULoadScreenWidget> LoadScreenWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadScreen> LoadScreenViewModelClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMVVM_LoadScreen> LoadScreenViewModel;

	UFUNCTION(BlueprintCallable)
	void ShowLoadScreen();

	UFUNCTION(BlueprintCallable)
	void HideLoadScreen();

#pragma endregion

#pragma region MainMenu
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UFatedBrandUserWidget> MainMenuWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UFatedBrandUserWidget> MainMenuWidget;

	UFUNCTION(BlueprintCallable)
	void ShowMainMenu();

	UFUNCTION(BlueprintCallable)
	void HideMainMenu();
#pragma endregion

#pragma region SettingMenu
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UFatedBrandUserWidget> SettingMenuClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UFatedBrandUserWidget> SettingMenu;

	UFUNCTION(BlueprintCallable)
	void ShowSettingMenu();

	UFUNCTION(BlueprintCallable)
	void HideSettingMenu();
#pragma endregion

protected:
	virtual void BeginPlay() override;

	FVector2D ViewportSize;
};
