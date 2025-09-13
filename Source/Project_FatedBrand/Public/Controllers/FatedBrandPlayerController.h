// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "FatedBrandPlayerController.generated.h"

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

protected :
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private :
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataAsset_InputConfig> InputConfigDataAsset;

	UPROPERTY()
	TObjectPtr<UFatedBrandAbilitySystemComponent> FatedBrandAbilitySystemComponent;
	
	UPROPERTY()
	TSoftObjectPtr<AFatedBrandCharacter> FatedBrandCharacter;

	

	void Input_Move(const FInputActionValue &InputActionValue);
	void Input_Jump(const FInputActionValue &InputActionValue);

	void Input_AbilityInputPressed(const FGameplayTag InInputTag);
	void Input_AbilityInputReleased(const FGameplayTag InInputTag);
};
