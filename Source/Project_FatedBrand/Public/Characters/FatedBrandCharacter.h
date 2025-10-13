// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FatedBrandStructTypes.h"
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

	UFUNCTION(BlueprintImplementableEvent)
	void WallJumping();

	UPROPERTY(BlueprintReadWrite)
	FDamageEffectParams SwordDamageEffectParams;

protected :
	virtual void PossessedBy(AController* NewController) override;

	virtual void InitAbilityActorInfo() override;

	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

private :
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;
};
