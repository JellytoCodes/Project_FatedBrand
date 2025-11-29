// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimInstance/PlayerAnimInstance.h"

#include "Characters/FatedBrandCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (!CachedFatedBrandCharacter.IsValid())
	{
		CachedFatedBrandCharacter = Cast<AFatedBrandCharacter>(TryGetPawnOwner());	
	}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (CachedFatedBrandCharacter.IsValid()) return;
	
	Velocity = CachedFatedBrandCharacter.Get()->GetCharacterMovement()->Velocity;
	GroundSpeed = UKismetMathLibrary::VSizeXY(Velocity);

	ShouldMove = CachedFatedBrandCharacter.Get()->GetCharacterMovement()->GetCurrentAcceleration() != FVector::ZeroVector && GroundSpeed > 0.01f;

	IsHanging = CachedFatedBrandCharacter.Get()->IsHanging;
	IsRestState = CachedFatedBrandCharacter.Get()->IsRestState;
}
