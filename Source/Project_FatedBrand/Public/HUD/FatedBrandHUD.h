// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FatedBrandHUD.generated.h"

class UInputMappingContext;
class UNebulaMenuWidgetController;
class UFatedBrandUserWidget;
class UAttributeSet;
class UAbilitySystemComponent;
class UOverlayWidgetController;

struct FWidgetControllerParams;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoveToLocationSignature, FVector2D, AxisVector);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSelectSocketFocusingSignature);

UCLASS()
class PROJECT_FATEDBRAND_API AFatedBrandHUD : public AHUD
{
	GENERATED_BODY()

public :
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UNebulaMenuWidgetController* GetNebulaMenuWidgetController(const FWidgetControllerParams& WCParams);
	void InitOverlay(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	void CreateNebulaMenu(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	void RemoveNebulaMenu();

	FORCEINLINE UInputMappingContext* GetWidgetMappingContext() { return WidgetMappingContext; }

	FOnMoveToLocationSignature OnMoveToLocationDelegate;

	FOnSelectSocketFocusingSignature OnSelectSocketFocusingDelegate;

private :
	UPROPERTY()
	TObjectPtr<UFatedBrandUserWidget> OverlayWidget;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass")
	TSubclassOf<UFatedBrandUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UFatedBrandUserWidget> NebulaWidget;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass")
	TSubclassOf<UFatedBrandUserWidget> NebulaWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UNebulaMenuWidgetController> NebulaWidgetController;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass")
	TSubclassOf<UNebulaMenuWidgetController> NebulaWidgetControllerClass;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetContext")
	TObjectPtr<UInputMappingContext> WidgetMappingContext;

	UFUNCTION()
	void MoveToAxis(const FVector2D InAxis);

	UFUNCTION()
	void SelectSocketFocusing();
};
