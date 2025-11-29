// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstance/FatedBrandAnimInstance.h"
#include "PlayerAnimInstance.generated.h"

class AFatedBrandCharacter;

UCLASS()
class PROJECT_FATEDBRAND_API UPlayerAnimInstance : public UFatedBrandAnimInstance
{
	GENERATED_BODY()

public :
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected :
	UPROPERTY(BlueprintReadWrite, Category = "OwnerState")
	bool ShouldMove = false;

	UPROPERTY(BlueprintReadWrite, Category = "OwnerState")
	bool IsFalling = false;

	UPROPERTY(BlueprintReadWrite, Category = "OwnerState")
	bool IsHanging = false;

	UPROPERTY(BlueprintReadWrite, Category = "OwnerState")
	bool IsRestState = false;

	TWeakObjectPtr<AFatedBrandCharacter> CachedFatedBrandCharacter;
};
