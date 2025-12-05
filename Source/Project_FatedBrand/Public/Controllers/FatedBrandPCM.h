// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "FatedBrandPCM.generated.h"

UCLASS()
class PROJECT_FATEDBRAND_API AFatedBrandPCM : public APlayerCameraManager
{
	GENERATED_BODY()
	
public:
	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, Category="Side Scrolling Camera", meta=(ClampMin=0, ClampMax=10000, Units="cm"))
	float CurrentZoom = 1200.0f;

	UPROPERTY(EditAnywhere, Category="Side Scrolling Camera", meta=(ClampMin=0, ClampMax=10000, Units="cm"))
	float CameraZOffset = 200.0f;

	UPROPERTY(EditAnywhere, Category="Side Scrolling Camera", meta=(ClampMin=-100000, ClampMax=100000, Units="cm"))
	float CameraXMinBounds = -6000.0f;

	UPROPERTY(EditAnywhere, Category="Side Scrolling Camera", meta=(ClampMin=-100000, ClampMax=100000, Units="cm"))
	float CameraXMaxBounds = 6000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Side Scrolling Camera")
	bool bPlayCinematic = false;

protected:
	float CurrentZ = 0.0f;

	bool bSetup = true;
	
};
