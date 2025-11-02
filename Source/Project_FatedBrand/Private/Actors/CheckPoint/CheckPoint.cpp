// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CheckPoint/CheckPoint.h"

#include "Components/BoxComponent.h"
#include "Game/FatedBrandGameModeBase.h"
#include "Interfaces/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

ACheckPoint::ACheckPoint(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetupAttachment(GetRootComponent());
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	CheckPointMesh = CreateDefaultSubobject<UStaticMeshComponent>("CheckPointMesh");
	CheckPointMesh->SetupAttachment(Box);
	CheckPointMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CheckPointMesh->SetCollisionResponseToAllChannels(ECR_Block);
}

void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();

	if (bBindOverlapCallback)
	{
		Box->OnComponentBeginOverlap.AddDynamic(this, &ACheckPoint::OnBoxOverlap);	
	}
}

void ACheckPoint::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		if (AFatedBrandGameModeBase* FatedBrandGameMode = Cast<AFatedBrandGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			const UWorld* CurrentWorld = GetWorld();
			FString MapName = CurrentWorld->GetMapName();
			MapName.RemoveFromStart(CurrentWorld->StreamingLevelsPrefix);

			FatedBrandGameMode->SaveProgressWorldState(GetWorld(), MapName);
		}
		IPlayerInterface::Execute_SaveProgress(OtherActor, PlayerStartTag);
	}
}

bool ACheckPoint::ShouldLoadTransform_Implementation()
{
	return ISaveInterface::ShouldLoadTransform_Implementation();
}

void ACheckPoint::LoadActor_Implementation()
{
	ISaveInterface::LoadActor_Implementation();
}