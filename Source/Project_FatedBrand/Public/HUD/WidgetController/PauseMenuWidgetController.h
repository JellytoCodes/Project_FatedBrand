// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/WidgetController/FatedBrandWidgetController.h"
#include "PauseMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectedMenu, int32, SelectNum);

UCLASS(BlueprintType, Blueprintable)
class PROJECT_FATEDBRAND_API UPauseMenuWidgetController : public UFatedBrandWidgetController
{
	GENERATED_BODY()

public :
	UPROPERTY(BlueprintAssignable)
	FOnSelectedMenu SelectedMenuDelegate;

	void SetSelectMenu(const int32 InAxisY);

	void EnteredInteraction();

private :
	int32 AxisY = -1;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> ConfirmSound;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> SelectSound;

	void QuitGame() const;
	void MainMenu() const;
	void Retry() const;
	void Resume();
};
