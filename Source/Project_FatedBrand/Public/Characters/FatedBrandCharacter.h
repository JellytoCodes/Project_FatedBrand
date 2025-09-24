// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/FatedBrandCharacterBase.h"
#include "FatedBrandCharacter.generated.h"

struct FInputActionValue;

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class PROJECT_FATEDBRAND_API AFatedBrandCharacter : public AFatedBrandCharacterBase
{
	GENERATED_BODY()

public :
	AFatedBrandCharacter();

protected :
	virtual void PossessedBy(AController* NewController) override;

	virtual void InitAbilityActorInfo() override;

private :
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StartUp|Abilities", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UFatedBrandGameplayAbility>> StartUpOffensiveAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StartUp|Abilities", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UFatedBrandGameplayAbility>> StartUpPassiveAbilities;
};
