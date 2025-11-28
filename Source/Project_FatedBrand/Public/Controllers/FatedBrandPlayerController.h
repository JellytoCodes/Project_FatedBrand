// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "FatedBrandPlayerController.generated.h"

class ICombatInterface;
class AFatedBrandHUD;
class UFatedBrandAbilitySystemComponent;
class AFatedBrandCharacter;
struct FInputActionValue;
class UDataAsset_InputConfig;

UCLASS()
class PROJECT_FATEDBRAND_API AFatedBrandPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public :
	virtual FGenericTeamId GetGenericTeamId() const override;

	UFatedBrandAbilitySystemComponent* GetFatedBrandASC();

	UFUNCTION(BlueprintCallable)
	void EnableDefaultMappingContext();

	UFUNCTION(BlueprintCallable)
	void DisableDefaultMappingContext();

	UFUNCTION(BlueprintCallable)
	void DisableHUD();

	void PauseMenuDisable();
	void AttributeMenuDisable();
	void SetIsCanCreateSaveMenu(const bool InIsCan) { bCanOpenSaveMenu = InIsCan; }
	void SetIsCanCreateAttributeMenu(const bool InIsCan) { bCanOpenAttributeMenu = InIsCan; }

protected :
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private :
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataAsset_InputConfig> InputConfigDataAsset;

	UPROPERTY()
	TObjectPtr<UFatedBrandAbilitySystemComponent> FatedBrandAbilitySystemComponent;
	
	UPROPERTY()
	TWeakObjectPtr<AFatedBrandCharacter> FatedBrandCharacter;

	void Input_Move(const FInputActionValue &InputActionValue);
	void Input_InteractUpKeyPressed();
	void Input_InteractUpKeyReleased();

	void Input_WidgetSelect();
	void Input_WidgetDeSelect();

	void Input_NebulaMenu();
	void Input_PauseMenu();

	void Input_AbilityInputPressed(const FGameplayTag InInputTag);
	void Input_AbilityInputReleased(const FGameplayTag InInputTag);
	void Input_AbilityInputHeld(const FGameplayTag InInputTag);

	void PlayerJump();
	void PlayerJumpEnd();

	UPROPERTY()
	TWeakObjectPtr<AFatedBrandHUD> CachedFatedBrandHUD;

	TWeakInterfacePtr<ICombatInterface> PlayerCombatInterface;

	bool bCanOpenSaveMenu = false;
	bool bCanOpenAttributeMenu = false;

	bool bNebulaMenuOpen = false;
	bool bPauseMenuOpen = false;
	bool bAttributeMenuOpen = false;

	bool bIsWidgetSelect = false;

	UPROPERTY(EditDefaultsOnly, Category="Input|Jump")
	float WallJumpTraceDistance = 200.0f;

	UPROPERTY(EditDefaultsOnly, Category="Input|Jump")
	float WallJumpHorizontalImpulse = 150.0f;

	UPROPERTY(EditDefaultsOnly, Category="Input|Jump")
	float WallJumpVerticalMultiplier = 1.7f;

	UPROPERTY(EditDefaultsOnly, Category="Input|Jump")
	float DelayBetweenWallJumps = 0.2f;

	UPROPERTY(EditAnywhere, Category="Input|Jump", meta = (ClampMin = 0, ClampMax = 5, Units = "s"))
	float MaxCoyoteTime = 0.16f;

	FTimerHandle WallJumpTimer;
	bool bHasWallJumped = false;
	bool bHasDoubleJumped = false;
	float ActionValueY = 0.0f;
	float LastFallTime = 0.0f;

	void ResetWallJump();

	void CanOpenWidget(bool &IsCanJump);
	void RestStateControl(bool &IsCanJump);

	bool CanKeyInput() const;
};
