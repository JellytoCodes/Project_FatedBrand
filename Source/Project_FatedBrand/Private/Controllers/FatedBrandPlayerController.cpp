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
}

void AFatedBrandPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	auto* FatedBrandEnhancedInputComponent = CastChecked<UFatedBrandEnhancedInputComponent>(InputComponent);

	FatedBrandEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, FatedBrandGameplayTags::Input_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	FatedBrandEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, FatedBrandGameplayTags::Input_Jump, ETriggerEvent::Started, this, &ThisClass::Input_Jump);

	FatedBrandEnhancedInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
}

void AFatedBrandPlayerController::Input_Move(const FInputActionValue &InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if(APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AFatedBrandPlayerController::Input_Jump(const FInputActionValue& InputActionValue)
{
	if (FatedBrandCharacter)
	{
		FatedBrandCharacter->Jump();
	}
}

void AFatedBrandPlayerController::Input_AbilityInputPressed(const FGameplayTag InInputTag)
{
	if (GetFatedBrandASC())
	{
		GetFatedBrandASC()->OnAbilityInputPressed(InInputTag);
	}
}

void AFatedBrandPlayerController::Input_AbilityInputReleased(const FGameplayTag InInputTag)
{
	if (GetFatedBrandASC())
	{
		GetFatedBrandASC()->OnAbilityInputReleased(InInputTag);
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