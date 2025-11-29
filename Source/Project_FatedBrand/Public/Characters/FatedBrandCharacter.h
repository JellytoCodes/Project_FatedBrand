// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/FatedBrandCharacterBase.h"
#include "Interfaces/PlayerInterface.h"
#include "FatedBrandCharacter.generated.h"

struct FGameplayEventData;
class AFatedBrandPlayerController;
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
	virtual void InteractSavePoint_Implementation(const bool IsInteraction) override;
	virtual void CanUpgradeAttribute_Implementation(const bool IsInteraction) override;
	virtual AFatedBrandPlayerState* GetPlayerState_Implementation() override;
	virtual void PlayerHideHUD_Implementation() override;
	virtual void ChangeToTravelState_Implementation() override;
	virtual bool CanRopeSwing_Implementation(const bool IsCanGrab) override;
	virtual void SetCanRest_Implementation(const bool InCanRest, AActor* InActor) override;
#pragma endregion

	void LoadProgress();

	UAttributeSet* GetAttributeSet() const { return FatedBrandAttributeSet; }

	UFUNCTION(BlueprintCallable)
	void InteractActorInApplyEffect(TSubclassOf<UGameplayEffect>EffectClass);

	UFUNCTION(BlueprintCallable)
	void InteractActorInRemoveEffect();

#pragma region CanState Definition
	UPROPERTY(BlueprintReadWrite)
	bool IsHanging = false;

	UPROPERTY(BlueprintReadWrite)
	bool IsRestState = false;

	bool CanShit = false;
#pragma endregion

protected :
	virtual void PossessedBy(AController* NewController) override;
	virtual void InitAbilityActorInfo() override;

private :
	UPROPERTY()
	TWeakObjectPtr<AFatedBrandPlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<AActor> InteractActor;

	FActiveGameplayEffectHandle InteractForActiveEffectHandle;
};
