// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SaveInterface.h"
#include "SpawnPoint.generated.h"

UCLASS()
class PROJECT_FATEDBRAND_API ASpawnPoint : public AActor, public ISaveInterface
{
	GENERATED_BODY()
	
public:	
	ASpawnPoint();

	virtual void LoadActor_Implementation() override;

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<AActor> SpawnClass;

	UPROPERTY(SaveGame)
	bool bReached = false;

	UPROPERTY(EditDefaultsOnly)
	bool bUsePersistence = true;

	TWeakObjectPtr<AActor> Spawned;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UBillboardComponent> Billboard;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USceneComponent> Scene;

protected :
	virtual void BeginPlay() override;
private :
	UFUNCTION()
	void OnSpawnedDestroyed(AActor* DestroyedActor);
};
