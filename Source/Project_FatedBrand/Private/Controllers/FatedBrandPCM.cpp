// Fill out your copyright notice in the Description page of Project Settings.

#include "Controllers/FatedBrandPCM.h"

void AFatedBrandPCM::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	if (bPlayCinematic)
	{
		Super::UpdateViewTarget(OutVT, DeltaTime);
		return;
	}

	if (IsValid(OutVT.Target))
	{
		OutVT.POV.Rotation = FRotator(0.0f, -90.0f, 0.0f);
		OutVT.POV.FOV = 65.0f;

		FVector CurrentActorLocation = OutVT.Target->GetActorLocation();
		FVector CurrentCameraLocation = GetCameraLocation();

		float CurrentY = CurrentZoom + CurrentActorLocation.Y;

		if (bSetup)
		{
			bSetup = false;

			OutVT.POV.Location.X = CurrentActorLocation.X;
			OutVT.POV.Location.Y = CurrentY;
			OutVT.POV.Location.Z = CurrentActorLocation.Z + CameraZOffset;

			CurrentZ = OutVT.POV.Location.Z;

			return;
		}

		bool bZUpdate = false;

		if (FMath::IsNearlyZero(OutVT.Target->GetVelocity().Z))
		{
			bZUpdate = FMath::IsNearlyEqual(CurrentZ, CurrentCameraLocation.Z, 25.0f);

		}
		else 
		{
			FHitResult OutHit;

			const FVector End = CurrentActorLocation + FVector(0.0f, 0.0f, -1000.0f);

			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(OutVT.Target);

			bZUpdate = !GetWorld()->LineTraceSingleByChannel(OutHit, CurrentActorLocation, End, ECC_Visibility, QueryParams);
		}

		if (bZUpdate)
		{
			CurrentZ = CurrentActorLocation.Z;

		}
		else 
		{
			if (FMath::IsNearlyEqual(CurrentZ, CurrentActorLocation.Z, 100.0f))
			{
				CurrentZ = CurrentActorLocation.Z;

			}
			else 
			{
				CurrentZ = FMath::FInterpTo(CurrentZ, CurrentActorLocation.Z, DeltaTime, 8.0f);
			}

		}

		float CurrentX = FMath::Clamp(CurrentActorLocation.X, CameraXMinBounds, CameraXMaxBounds);

		FVector TargetCameraLocation(CurrentX, CurrentY, CurrentZ);

		OutVT.POV.Location = FMath::VInterpTo(CurrentCameraLocation, TargetCameraLocation, DeltaTime, 2.0f);
	}
}