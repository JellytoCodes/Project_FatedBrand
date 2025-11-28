// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CheckPoint/NextDestinationPoint.h"

#include "Game/FatedBrandGameModeBase.h"
#include "Interfaces/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

void ANextDestinationPoint::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		if (AFatedBrandGameModeBase* FatedBrandGameMode = Cast<AFatedBrandGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			FatedBrandGameMode->SaveProgressWorldState(GetWorld(), DestinationMap.ToSoftObjectPath().GetAssetName());
		}
		IPlayerInterface::Execute_ChangeToTravelState(OtherActor);
		IPlayerInterface::Execute_SaveProgress(OtherActor, DestinationPlayerStartTag);
		IPlayerInterface::Execute_MapEndPlayMontage(OtherActor, DestinationMap);
	}
}
