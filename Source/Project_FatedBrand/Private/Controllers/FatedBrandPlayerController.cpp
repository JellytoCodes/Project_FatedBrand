// Fill out your copyright notice in the Description page of Project Settings.

#include "Controllers/FatedBrandPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/FatedBrandAbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FatedBrandGameplayTags.h"
#include "Characters/FatedBrandCharacter.h"
#include "Components/Input/FatedBrandEnhancedInputComponent.h"
#include "DataAssets/DataAsset_InputConfig.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/FatedBrandHUD.h"
#include "Project_FatedBrand/Project_FatedBrand.h"

FGenericTeamId AFatedBrandPlayerController::GetGenericTeamId() const
{
	return FGenericTeamId(0);
}

void AFatedBrandPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(InputConfigDataAsset);

	if (GetFatedBrandASC())
	{
		FatedBrandCharacter = Cast<AFatedBrandCharacter>(GetFatedBrandASC()->GetAvatarActor());
	}

	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);
	}

	if (!CachedFatedBrandHUD.IsValid())
	{
		CachedFatedBrandHUD = Cast<AFatedBrandHUD>(GetHUD());
	}
}

void AFatedBrandPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	auto* FatedBrandEnhancedInputComponent = CastChecked<UFatedBrandEnhancedInputComponent>(InputComponent);

	FatedBrandEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, FatedBrandGameplayTags::Input_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	FatedBrandEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, FatedBrandGameplayTags::Input_Jump, ETriggerEvent::Started, this, &ThisClass::Input_JumpStart);
	FatedBrandEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, FatedBrandGameplayTags::Input_Jump, ETriggerEvent::Completed, this, &ThisClass::Input_JumpEnd);
	FatedBrandEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, FatedBrandGameplayTags::Input_NebulaMenu, ETriggerEvent::Started, this, &ThisClass::Input_NebulaMenu);
	FatedBrandEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, FatedBrandGameplayTags::Input_WidgetSelect, ETriggerEvent::Started, this, &ThisClass::Input_WidgetSelect);

	FatedBrandEnhancedInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased, &ThisClass::Input_AbilityInputHeld);
}

void AFatedBrandPlayerController::Input_Move(const FInputActionValue &InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	if (bIsNebulaMenu)
	{
		CachedFatedBrandHUD->OnMoveToLocationDelegate.Broadcast(InputAxisVector);
	}

	else
	{
		if (!bHasWallJumped)
		{
			ActionValueY = InputAxisVector.Y;

			const FVector MoveDir = FVector(1.0f, 0.0f, 0.0f);

			if(APawn* ControlledPawn = GetPawn<APawn>())
			{
				ControlledPawn->AddMovementInput(MoveDir, InputAxisVector.Y);
			}
		}	
	}
}

void AFatedBrandPlayerController::Input_JumpStart()
{
	if (bIsNebulaMenu) return;

	if (FatedBrandCharacter.IsValid())
	{
		if (!FatedBrandCharacter->GetCharacterMovement()->IsFalling())
		{
			FatedBrandCharacter->Jump();
			return;
		}

		if (!bHasWallJumped && !FMath::IsNearlyZero(ActionValueY))
		{
			FHitResult OutHit;

			const FVector Start = FatedBrandCharacter->GetActorLocation();
			const FVector End = Start + (FVector(ActionValueY > 0.0f ? 1.0f : -1.0f, 0.0f, 0.0f) * WallJumpTraceDistance);

			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(this);

			GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, QueryParams);

			if (OutHit.bBlockingHit)
			{
				const FRotator BounceRot = UKismetMathLibrary::MakeRotFromX(OutHit.ImpactNormal);
				FatedBrandCharacter->SetActorRotation(FRotator(0.0f, BounceRot.Yaw, 0.0f));

				FVector WallJumpImpulse = OutHit.ImpactNormal * WallJumpHorizontalImpulse;
				WallJumpImpulse.Z = FatedBrandCharacter->GetCharacterMovement()->JumpZVelocity * WallJumpVerticalMultiplier;

				FatedBrandCharacter->LaunchCharacter(WallJumpImpulse, true, true);

				bHasWallJumped = true;
				FatedBrandCharacter->WallJumping();

				GetWorld()->GetTimerManager().SetTimer(WallJumpTimer, this, &ThisClass::ResetWallJump, DelayBetweenWallJumps, false);

				return;
			}
		}

		if (!bHasWallJumped)
		{
			if (GetWorld()->GetTimeSeconds() - LastFallTime < MaxCoyoteTime)
			{
				FatedBrandCharacter->Jump();
			}
			else 
			{
				if (!bHasDoubleJumped)
				{
					bHasDoubleJumped = true;

					FatedBrandCharacter->Jump();
				}
			}
		}
	}
}
void AFatedBrandPlayerController::ResetWallJump()
{
	bHasWallJumped = false;
}

void AFatedBrandPlayerController::Input_JumpEnd()
{
	if (FatedBrandCharacter.IsValid())
	{
		FatedBrandCharacter->StopJumping();

		if (bHasDoubleJumped) bHasDoubleJumped = false;
	}
}

void AFatedBrandPlayerController::Input_WidgetSelect()
{
	if (CachedFatedBrandHUD.IsValid())
	{
		CachedFatedBrandHUD->OnSelectSocketFocusingDelegate.Broadcast();
	}
}

void AFatedBrandPlayerController::Input_NebulaMenu()
{
	if (FatedBrandCharacter == nullptr) return;

	if (CachedFatedBrandHUD.IsValid())
	{
		auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
		
		if (!bIsNebulaMenu)
		{
			CachedFatedBrandHUD->CreateNebulaMenu(this, FatedBrandCharacter->GetAbilitySystemComponent(), FatedBrandCharacter->GetFatedBrandAttributeSet());
			bIsNebulaMenu = true;
			if (Subsystem) Subsystem->AddMappingContext(CachedFatedBrandHUD->GetWidgetMappingContext(), 1);
		}
		else
		{
			CachedFatedBrandHUD->RemoveNebulaMenu();
			bIsNebulaMenu = false;
			if (Subsystem) Subsystem->RemoveMappingContext(CachedFatedBrandHUD->GetWidgetMappingContext());
		}
	}

}

void AFatedBrandPlayerController::Input_AbilityInputPressed(const FGameplayTag InInputTag)
{
	if (bIsNebulaMenu) return;

	if (GetFatedBrandASC())
	{
		GetFatedBrandASC()->OnAbilityInputPressed(InInputTag);
	}
}

void AFatedBrandPlayerController::Input_AbilityInputReleased(const FGameplayTag InInputTag)
{
	if (bIsNebulaMenu) return;

	if (GetFatedBrandASC())
	{
		GetFatedBrandASC()->OnAbilityInputReleased(InInputTag);
	}
}

void AFatedBrandPlayerController::Input_AbilityInputHeld(const FGameplayTag InInputTag)
{
	if (bIsNebulaMenu) return;

	if (GetFatedBrandASC())
	{
		GetFatedBrandASC()->OnAbilityInputHeld(InInputTag);
	}
}

UFatedBrandAbilitySystemComponent* AFatedBrandPlayerController::GetFatedBrandASC()
{
	if (FatedBrandAbilitySystemComponent == nullptr)
	{
		FatedBrandAbilitySystemComponent = Cast<UFatedBrandAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return FatedBrandAbilitySystemComponent;
}
