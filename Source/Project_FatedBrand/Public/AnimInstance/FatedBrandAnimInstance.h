// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FatedBrandAnimInstance.generated.h"

UCLASS()
class PROJECT_FATEDBRAND_API UFatedBrandAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected :
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	FVector Velocity = FVector::Zero();

	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	float GroundSpeed = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	float Direction = 0.f;

};
