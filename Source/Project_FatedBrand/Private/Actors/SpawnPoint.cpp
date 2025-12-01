// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SpawnPoint.h"

#include "Components/BillboardComponent.h"
#include "Components/SceneComponent.h"

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
		// 보스 스테이지 클리어 후 레벨을 넘나들기 위한 용도
		SpawnedDestroyedDelegate.Broadcast();

		Destroy();
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
		Spawned = GetWorld()->SpawnActorDeferred<AActor>(Cls, GetActorTransform());
		if (Spawned.IsValid())
		{
			Spawned->FinishSpawning(GetActorTransform());
			Spawned->OnDestroyed.AddUniqueDynamic(this, &ThisClass::OnSpawnedDestroyed);
		}
	}
}

void ASpawnPoint::OnSpawnedDestroyed(AActor* DestroyedActor)
{
	// 보스 스테이지 클리어 시 레벨 내 오브젝트와 상호작용
	SpawnedDestroyedDelegate.Broadcast();

	if (bUsePersistence) bReached = true;
}

