// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/CheckPoint/CheckPoint.h"
#include "AttributeUpgradePoint.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_FATEDBRAND_API AAttributeUpgradePoint : public ACheckPoint
{
	GENERATED_BODY()

protected :
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
