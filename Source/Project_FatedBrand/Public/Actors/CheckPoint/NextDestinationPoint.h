// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/CheckPoint/CheckPoint.h"
#include "NextDestinationPoint.generated.h"

UCLASS()
class PROJECT_FATEDBRAND_API ANextDestinationPoint : public ACheckPoint
{
	GENERATED_BODY()

public :
	UPROPERTY(EditAnywhere, Category = "DestinationMap")
	TSoftObjectPtr<UWorld> DestinationMap;

	UPROPERTY(EditAnywhere, Category = "DestinationMap")
	FName DestinationPlayerStartTag;

protected :
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
