// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interfaces/SaveInterface.h"
#include "CheckPoint.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSaveWidgetOpen);

UCLASS()
class PROJECT_FATEDBRAND_API ACheckPoint : public APlayerStart, public ISaveInterface
{
	GENERATED_BODY()

public :
	ACheckPoint(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere)
	bool bBindOverlapCallback = true;

protected :
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> CheckPointMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Box;
};
