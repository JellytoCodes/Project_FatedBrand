// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/FatedBrandCharacterBase.h"
#include "Interfaces/PlayerInterface.h"
#include "FatedBrandCharacter.generated.h"

struct FInputActionValue;

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class PROJECT_FATEDBRAND_API AFatedBrandCharacter : public AFatedBrandCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public :
	AFatedBrandCharacter();

	UFUNCTION(BlueprintImplementableEvent)
	void WallJumping();

#pragma region Player Interface
	virtual void UpdateAbilities_Implementation(const FGameplayTag& AbilityTag) override;
	virtual void SaveProgress_Implementation(const FName& CheckPointTag) override;
	virtual float GetVitalSurgeGage_Implementation() override;
#pragma endregion

	void LoadProgress();

protected :
	virtual void PossessedBy(AController* NewController) override;

	virtual void InitAbilityActorInfo() override;

private :
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;
};
