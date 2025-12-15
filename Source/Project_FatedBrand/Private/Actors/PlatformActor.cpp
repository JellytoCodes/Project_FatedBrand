// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/PlatformActor.h"

#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetMathLibrary.h"

APlatformActor::APlatformActor()
{
	PrimaryActorTick.bCanEverTick = true;

	PlatformCollision = CreateDefaultSubobject<UBoxComponent>("PlatformCollision");
	PlatformCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	PlatformCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	SetRootComponent(PlatformCollision);

	DestinationPoint = CreateDefaultSubobject<USceneComponent>("DestinationPoint");
	DestinationPoint->SetupAttachment(PlatformCollision);

	ViewDestinationPoint = CreateDefaultSubobject<UBillboardComponent>("ViewDestinationPoint");
	ViewDestinationPoint->SetupAttachment(DestinationPoint);
}

void APlatformActor::BeginPlay()
{
	Super::BeginPlay();

	OriginLocation = PlatformCollision->GetRelativeLocation();
	DestinationLocation = DestinationPoint->K2_GetComponentLocation();
}

void APlatformActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ActivateGoingDestination();
}

void APlatformActor::ActivateGoingDestination()
{
	if (IsGoingDestination)
	{
		DistTo = UKismetMathLibrary::VSize(PlatformCollision->GetRelativeLocation() - DestinationLocation);
		PlatformCollision->SetRelativeLocation(PlatformCollision->GetRelativeLocation() + DurationLocation);
		if (DistTo < 10)
		{
			IsGoingDestination = false;
		}
	}
	if (!IsGoingDestination)
	{
		DistTo = UKismetMathLibrary::VSize(PlatformCollision->GetRelativeLocation() - OriginLocation);
		PlatformCollision->SetRelativeLocation(PlatformCollision->GetRelativeLocation() + (DurationLocation * -1));
		if (DistTo < 10)
		{
			IsGoingDestination = true;
		}
	}
}
