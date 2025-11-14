// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlatformActor.generated.h"

class UBoxComponent;

UCLASS()
class PROJECT_FATEDBRAND_API APlatformActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APlatformActor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private :
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> PlatformCollision;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DestinationPoint;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBillboardComponent> ViewDestinationPoint;

	FVector OriginLocation = FVector();

	FVector DestinationLocation = FVector();

	UPROPERTY(EditAnywhere, Category = "Duration", meta = (AllowPrivateAccess = "true", ClampMin = -5.f, ClampMax = 5.f))
	FVector DurationLocation = FVector();

	bool IsGoingDestination = true;

	float DistTo = 0.f;

	void ActivateGoingDestination();
};
