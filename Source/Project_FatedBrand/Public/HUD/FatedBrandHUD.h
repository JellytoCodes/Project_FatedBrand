// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FatedBrandHUD.generated.h"

class UAttributeMenuWidgetController;
class UPauseMenuWidgetController;
class UMVVM_LoadScreen;
class ULoadScreenWidget;
class UInputMappingContext;
class UNebulaMenuWidgetController;
class UFatedBrandUserWidget;
class UAttributeSet;
class UAbilitySystemComponent;
class UOverlayWidgetController;

struct FWidgetControllerParams;

UCLASS()
class PROJECT_FATEDBRAND_API AFatedBrandHUD : public AHUD
{
	GENERATED_BODY()

public :
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UNebulaMenuWidgetController* GetNebulaMenuWidgetController(const FWidgetControllerParams& WCParams);
	UPauseMenuWidgetController* GetPauseMenuWidgetController(const FWidgetControllerParams& WCParams);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	void HideOverlay();

	void VisibleNebulaMenu(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	void HideNebulaMenu();

	void VisiblePauseMenu(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	void HidePauseMenu();

	UFUNCTION(BlueprintCallable)
	void CreateSaveMenuWidget();

	UFUNCTION(BlueprintCallable)
	void CreateAttributeMenuWidget();

	void RemoveAttributeMenuWidget();

	FORCEINLINE UInputMappingContext* GetWidgetMappingContext() { return WidgetMappingContext; }

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULoadScreenWidget> SaveScreenWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULoadScreenWidget> SaveScreenWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadScreen> SaveScreenViewModelClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMVVM_LoadScreen> SaveScreenViewModel;



private :
	UPROPERTY(EditDefaultsOnly, Category = "WidgetContext")
	TObjectPtr<UInputMappingContext> WidgetMappingContext;

#pragma region OverlayWidget
	UPROPERTY()
	TObjectPtr<UFatedBrandUserWidget> OverlayWidget;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass")
	TSubclassOf<UFatedBrandUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
#pragma endregion

#pragma region NebulaWidget
	UPROPERTY()
	TObjectPtr<UFatedBrandUserWidget> NebulaWidget;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass")
	TSubclassOf<UFatedBrandUserWidget> NebulaWidgetClass;

	UPROPERTY()
	TObjectPtr<UNebulaMenuWidgetController> NebulaWidgetController;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass")
	TSubclassOf<UNebulaMenuWidgetController> NebulaWidgetControllerClass;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass")
	FVector2D NebulaWidgetPosition = FVector2D();
#pragma endregion

#pragma region PauseWidget
	UPROPERTY()
	TObjectPtr<UFatedBrandUserWidget> PauseWidget;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass")
	TSubclassOf<UFatedBrandUserWidget> PauseWidgetClass;

	UPROPERTY()
	TObjectPtr<UPauseMenuWidgetController> PauseWidgetController;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass")
	TSubclassOf<UPauseMenuWidgetController> PauseWidgetControllerClass;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass")
	FVector2D PauseWidgetPosition = FVector2D();
#pragma endregion

#pragma region AttributeWidget
	UPROPERTY()
	TObjectPtr<UFatedBrandUserWidget> AttributeWidget;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass")
	TSubclassOf<UFatedBrandUserWidget> AttributeWidgetClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass")
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
#pragma  endregion
};
