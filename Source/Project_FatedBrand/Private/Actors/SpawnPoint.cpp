// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SpawnPoint.h"

#include "Components/BillboardComponent.h"

ASpawnPoint::ASpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	SetRootComponent(Scene);

	Billboard = CreateDefaultSubobject<UBillboardComponent>("Billboard");
	Billboard->SetupAttachment(GetRootComponent());
}

void ASpawnPoint::LoadActor_Implementation()
{
	if (bUsePersistence && bReached)
	{
		Destroy();
		return;
	}
}

void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	if (bUsePersistence && bReached) return;

	if (UClass* Cls = SpawnClass.LoadSynchronous())
	{
		FActorSpawnParameters SpawnParam;

		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		Spawned = GetWorld()->SpawnActor<AActor>(Cls, GetActorTransform(), SpawnParam);
		if (Spawned.IsValid())
		{
			Spawned->OnDestroyed.AddUniqueDynamic(this, &ThisClass::OnSpawnedDestroyed);
		}
	}
}

void ASpawnPoint::OnSpawnedDestroyed(AActor* DestroyedActor)
{
	if (bUsePersistence) bReached = true;
}

