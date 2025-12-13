// Fill out your copyright notice in the Description page of Project Settings.

#include "Controllers/FatedBrandPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/FatedBrandAbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FatedBrandFunctionLibrary.h"
#include "FatedBrandGameplayTags.h"
#include "Characters/FatedBrandCharacter.h"
#include "Components/Input/FatedBrandEnhancedInputComponent.h"
#include "DataAssets/DataAsset_InputConfig.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/FatedBrandHUD.h"
#include "HUD/WidgetController/AttributeMenuWidgetController.h"
#include "HUD/WidgetController/NebulaMenuWidgetController.h"
#include "HUD/WidgetController/PauseMenuWidgetController.h"

AFatedBrandPlayerController::AFatedBrandPlayerController()
{
	PlayerTeamID = FGenericTeamId(0);
}

FGenericTeamId AFatedBrandPlayerController::GetGenericTeamId() const
{
	return PlayerTeamID;
}

void AFatedBrandPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(InputConfigDataAsset);

	if (GetFatedBrandASC()) FatedBrandCharacter = Cast<AFatedBrandCharacter>(GetFatedBrandASC()->GetAvatarActor());

	if (!CachedFatedBrandHUD.IsValid()) CachedFatedBrandHUD = Cast<AFatedBrandHUD>(GetHUD());

	PlayerCombatInterface = TWeakInterfacePtr<ICombatInterface>(GetOwner());

	bShowMouseCursor = false;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameOnly InputModeData;
	InputModeData.SetConsumeCaptureMouseDown(false);
	SetInputMode(InputModeData);
}

void AFatedBrandPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	auto* FatedBrandEnhancedInputComponent = CastChecked<UFatedBrandEnhancedInputComponent>(InputComponent);

	FatedBrandEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, FatedBrandGameplayTags::Input_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	FatedBrandEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, FatedBrandGameplayTags::Input_Jump, ETriggerEvent::Started, this, &ThisClass::Input_InteractUpKeyPressed);
	FatedBrandEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, FatedBrandGameplayTags::Input_Jump, ETriggerEvent::Completed, this, &ThisClass::Input_InteractUpKeyReleased);
	FatedBrandEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, FatedBrandGameplayTags::Input_NebulaMenu, ETriggerEvent::Started, this, &ThisClass::Input_NebulaMenu);
	FatedBrandEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, FatedBrandGameplayTags::Input_PauseMenu, ETriggerEvent::Started, this, &ThisClass::Input_PauseMenu);
	FatedBrandEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, FatedBrandGameplayTags::Input_KeyDescription, ETriggerEvent::Started, this, &ThisClass::Input_KeyDescription);
	FatedBrandEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, FatedBrandGameplayTags::Input_WidgetSelect, ETriggerEvent::Started, this, &ThisClass::Input_WidgetSelect);
	FatedBrandEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, FatedBrandGameplayTags::Input_WidgetDeSelect, ETriggerEvent::Started, this, &ThisClass::Input_WidgetDeSelect);

	FatedBrandEnhancedInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased, &ThisClass::Input_AbilityInputHeld);
}

void AFatedBrandPlayerController::Input_Move(const FInputActionValue &InputActionValue)
{
	if (FatedBrandCharacter->IsRestState) return;

	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	if (FatedBrandCharacter->GetAbilitySystemComponent()->HasMatchingGameplayTag(FatedBrandGameplayTags::Ability_Activate_VitalSurge)) return;

	switch (WidgetOpenState)
	{
	case EWidgetOpenState::None :
		if (!bHasWallJumped)
		{
			ActionValueY = InputAxisVector.Y;

			const FVector MoveDir = FVector(1.0f, 0.0f, 0.0f);

			if(APawn* ControlledPawn = GetPawn<APawn>())
			{
				ControlledPawn->AddMovementInput(MoveDir, InputAxisVector.Y);
			}
		}
		break;

	case EWidgetOpenState::NebulaMenu :
		UFatedBrandFunctionLibrary::GetNebulaMenuWidgetController(this)->SetSelectSocketAxis(InputAxisVector.X, InputAxisVector.Y);
		break;

	case EWidgetOpenState::PauseMenu :
		UFatedBrandFunctionLibrary::GetPauseMenuWidgetController(this)->SetSelectMenu(InputAxisVector.Y);
		break;

	case EWidgetOpenState::AttributeMenu :
		UFatedBrandFunctionLibrary::GetAttributeMenuWidgetController(this)->WidgetAxisControl(InputAxisVector.X, InputAxisVector.Y);
		break;
	}
}

void AFatedBrandPlayerController::Input_InteractUpKeyPressed()
{
	if (FatedBrandCharacter->GetAbilitySystemComponent()->HasMatchingGameplayTag(FatedBrandGameplayTags::Ability_Activate_VitalSurge)) return;
	if (WidgetOpenState != EWidgetOpenState::None || FatedBrandCharacter->IsHanging) return;

	bool IsInteractUpKeyCanJump = true;

	CanOpenWidget(IsInteractUpKeyCanJump);
	RestStateControl(IsInteractUpKeyCanJump);

	if (IsInteractUpKeyCanJump) PlayerJump();
}

void AFatedBrandPlayerController::CanOpenWidget(bool& IsCanJump)
{
	if (!CachedFatedBrandHUD.IsValid()) return;

	auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	switch (CanCreateWidget)
	{
	case ECanCreateWidget::None :
		IsCanJump = true;
		break;
	case ECanCreateWidget::SaveMenu :
		CachedFatedBrandHUD->CreateSaveMenuWidget();
		IsCanJump = false;
		break;
	case ECanCreateWidget::AttributeMenu :
		CachedFatedBrandHUD->CreateAttributeMenuWidget();
		WidgetOpenState = EWidgetOpenState::AttributeMenu;
		IsCanJump = false;
		Subsystem->AddMappingContext(CachedFatedBrandHUD->GetWidgetMappingContext(), 1);
		break;
	}
}

void AFatedBrandPlayerController::RestStateControl(bool& IsCanJump)
{
	if (FatedBrandCharacter->CanShit == false) return;

	if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(FatedBrandCharacter.Get()))
	{
		IPlayerInterface::Execute_RestStateInKey(FatedBrandCharacter.Get());
		IsCanJump = false;
	}
}

bool AFatedBrandPlayerController::CanKeyInput() const
{
	if (WidgetOpenState != EWidgetOpenState::None) return true;
	if (FatedBrandCharacter->IsHanging) return true;
	if (FatedBrandCharacter->IsRestState) return true;

	return false;
}

void AFatedBrandPlayerController::Input_InteractUpKeyReleased()
{
	if (CanKeyInput()) return;

	PlayerJumpEnd();
}

void AFatedBrandPlayerController::Input_WidgetSelect()
{
	UNebulaMenuWidgetController* NebulaMenuWidgetController = UFatedBrandFunctionLibrary::GetNebulaMenuWidgetController(this);

	switch (WidgetOpenState)
	{
	case EWidgetOpenState::NebulaMenu :
		if (!bIsWidgetSelect)
		{
			NebulaMenuWidgetController->SelectSocketFocusingController();
		}
		else
		{
			NebulaMenuWidgetController->SelectSocketConfirm();
		}
		bIsWidgetSelect = !bIsWidgetSelect;	
		break;
	case EWidgetOpenState::PauseMenu :
		UFatedBrandFunctionLibrary::GetPauseMenuWidgetController(this)->EnteredInteraction();
		break;
	case EWidgetOpenState::AttributeMenu :
		UFatedBrandFunctionLibrary::GetAttributeMenuWidgetController(this)->InteractEnterDelegate.Broadcast();
		break;
	}
}

void AFatedBrandPlayerController::Input_WidgetDeSelect()
{
	if (FatedBrandCharacter == nullptr) return;

	if (WidgetOpenState == EWidgetOpenState::NebulaMenu && bIsWidgetSelect)
	{
		UNebulaMenuWidgetController* NebulaMenuWidgetController = UFatedBrandFunctionLibrary::GetNebulaMenuWidgetController(this);
		NebulaMenuWidgetController->SelectSocketFocusingController();
		bIsWidgetSelect = !bIsWidgetSelect;
	}

	AttributeMenuDisable();
	PauseMenuDisable();	
}

void AFatedBrandPlayerController::Input_NebulaMenu()
{
	if (FatedBrandCharacter == nullptr || (WidgetOpenState != EWidgetOpenState::None && WidgetOpenState != EWidgetOpenState::NebulaMenu)) return;

	if (CachedFatedBrandHUD.IsValid())
	{
		auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
		
		if (WidgetOpenState != EWidgetOpenState::NebulaMenu)
		{
			CachedFatedBrandHUD->VisibleNebulaMenu(this, FatedBrandCharacter->GetAbilitySystemComponent(), FatedBrandCharacter->GetAttributeSet());
			WidgetOpenState = EWidgetOpenState::NebulaMenu;
			if (Subsystem) Subsystem->AddMappingContext(CachedFatedBrandHUD->GetWidgetMappingContext(), 1);
		}
		else
		{
			CachedFatedBrandHUD->HideNebulaMenu();
			WidgetOpenState = EWidgetOpenState::None;
			bIsWidgetSelect = false;
			if (Subsystem) Subsystem->RemoveMappingContext(CachedFatedBrandHUD->GetWidgetMappingContext());
		}
	}
}

void AFatedBrandPlayerController::Input_PauseMenu()
{
	if (FatedBrandCharacter == nullptr || (WidgetOpenState != EWidgetOpenState::None && WidgetOpenState != EWidgetOpenState::PauseMenu)) return;

	if (CachedFatedBrandHUD.IsValid())
	{
		auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
		if (WidgetOpenState != EWidgetOpenState::PauseMenu)
		{
			CachedFatedBrandHUD->VisiblePauseMenu(this, FatedBrandCharacter->GetAbilitySystemComponent(), FatedBrandCharacter->GetAttributeSet());
			WidgetOpenState = EWidgetOpenState::PauseMenu;
			if (Subsystem) Subsystem->AddMappingContext(CachedFatedBrandHUD->GetWidgetMappingContext(), 1);
		}
		else
		{
			CachedFatedBrandHUD->HidePauseMenu();
			WidgetOpenState = EWidgetOpenState::None;
			if (Subsystem) Subsystem->RemoveMappingContext(CachedFatedBrandHUD->GetWidgetMappingContext());
		}
	}
}

void AFatedBrandPlayerController::Input_KeyDescription()
{
	if (FatedBrandCharacter == nullptr || (WidgetOpenState != EWidgetOpenState::None && WidgetOpenState != EWidgetOpenState::KeyDescription)) return;
	if (CachedFatedBrandHUD.IsValid())
	{
		if (WidgetOpenState != EWidgetOpenState::KeyDescription)
		{
			CachedFatedBrandHUD->CreateKeyDescriptionWidget();
			WidgetOpenState = EWidgetOpenState::KeyDescription;
		}
		else
		{
			CachedFatedBrandHUD->RemoveKeyDescriptionWidget();
			WidgetOpenState = EWidgetOpenState::None;
		}
	}
}

void AFatedBrandPlayerController::PauseMenuDisable()
{
	if (WidgetOpenState != EWidgetOpenState::PauseMenu || ! CachedFatedBrandHUD.IsValid()) return;

	auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	CachedFatedBrandHUD->HidePauseMenu();
	WidgetOpenState = EWidgetOpenState::None;
	if (Subsystem) Subsystem->RemoveMappingContext(CachedFatedBrandHUD->GetWidgetMappingContext());
}

void AFatedBrandPlayerController::AttributeMenuDisable()
{
	if (WidgetOpenState != EWidgetOpenState::AttributeMenu || ! CachedFatedBrandHUD.IsValid()) return;

	auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	CachedFatedBrandHUD->RemoveAttributeMenuWidget();
	WidgetOpenState = EWidgetOpenState::None;
	if (Subsystem) Subsystem->RemoveMappingContext(CachedFatedBrandHUD->GetWidgetMappingContext());
}

void AFatedBrandPlayerController::Input_AbilityInputPressed(const FGameplayTag InInputTag)
{
	if (CanKeyInput()) return;

	if (GetFatedBrandASC())
	{
		GetFatedBrandASC()->OnAbilityInputPressed(InInputTag);
	}
}

void AFatedBrandPlayerController::Input_AbilityInputReleased(const FGameplayTag InInputTag)
{
	if (CanKeyInput()) return;

	if (GetFatedBrandASC())
	{
		GetFatedBrandASC()->OnAbilityInputReleased(InInputTag);
	}
}

void AFatedBrandPlayerController::Input_AbilityInputHeld(const FGameplayTag InInputTag)
{
	if (CanKeyInput()) return;

	if (GetFatedBrandASC())
	{
		GetFatedBrandASC()->OnAbilityInputHeld(InInputTag);
	}
}

void AFatedBrandPlayerController::PlayerJump()
{
	if (FatedBrandCharacter.IsValid())
	{
		if (FatedBrandCharacter->IsHanging) return;

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

void AFatedBrandPlayerController::PlayerJumpEnd()
{
	if (FatedBrandCharacter.IsValid())
	{
		FatedBrandCharacter->StopJumping();

		if (bHasDoubleJumped) bHasDoubleJumped = false;
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

void AFatedBrandPlayerController::EnableDefaultMappingContext()
{
	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);	
	}
}

void AFatedBrandPlayerController::DisableDefaultMappingContext()
{
	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->RemoveMappingContext(InputConfigDataAsset->DefaultMappingContext);	
	}
}

void AFatedBrandPlayerController::DisableHUD()
{
	if (CachedFatedBrandHUD.IsValid())
	{
		CachedFatedBrandHUD->HideOverlay();
	}
}
