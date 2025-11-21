// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/FatedBrandCharacter.h"

#include "FatedBrandFunctionLibrary.h"
#include "AbilitySystem/FatedBrandAbilitySystemComponent.h"
#include "AbilitySystem/FatedBrandAttributeSet.h"
#include "Camera/CameraComponent.h"
#include "Controllers/FatedBrandPlayerController.h"
#include "DataAssets/DataAsset_AbilityInfo.h"
#include "Game/FatedBrandGameModeBase.h"
#include "Game/FatedBrandPlayerState.h"
#include "Game/FatedBrandSaveGame.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "HUD/FatedBrandHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Game/FatedBrandPlayerState.h"
#include "Project_FatedBrand/Project_FatedBrand.h"

AFatedBrandCharacter::AFatedBrandCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 200.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->PerchRadiusThreshold = 15.0f;
	GetCharacterMovement()->LedgeCheckThreshold = 6.0f;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 4000.f, 0.f);

	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, 1.0f, 0.0f));
	GetCharacterMovement()->bConstrainToPlane = true;
}

void AFatedBrandCharacter::UpdateAbilities_Implementation(const FGameplayTag& AbilityTag)
{
	UFatedBrandAbilitySystemComponent* FatedBrandASC = CastChecked<UFatedBrandAbilitySystemComponent>(FatedBrandAbilitySystemComponent);
	FatedBrandASC->UpdateAbilityStatuses(AbilityTag);
}

void AFatedBrandCharacter::SaveProgress_Implementation(const FName& CheckPointTag)
{
	if (AFatedBrandGameModeBase* FatedBrandGameMode = Cast<AFatedBrandGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		UFatedBrandSaveGame* SaveData = FatedBrandGameMode->GetProgressSaveData();
		if (SaveData == nullptr) return;

		SaveData->PlayerStartTag = CheckPointTag;

		SaveData->MaxHealth = UFatedBrandAttributeSet::GetMaxHealthAttribute().GetNumericValue(FatedBrandAttributeSet);
		SaveData->CurrentHealth = UFatedBrandAttributeSet::GetCurrentHealthAttribute().GetNumericValue(FatedBrandAttributeSet);
		SaveData->AttackPower = UFatedBrandAttributeSet::GetAttackPowerAttribute().GetNumericValue(FatedBrandAttributeSet);
		SaveData->VitalSurge = UFatedBrandAttributeSet::GetVitalSurgeAttribute().GetNumericValue(FatedBrandAttributeSet);

		SaveData->bFirstTimeLoadIn = false;

		UFatedBrandAbilitySystemComponent* FatedBrandASC = Cast<UFatedBrandAbilitySystemComponent>(GetAbilitySystemComponent());
		FForEachAbility SaveAbilityDelegate;
		SaveData->SavedAbilities.Empty();
		SaveAbilityDelegate.BindLambda([this, FatedBrandASC, SaveData](const FGameplayAbilitySpec& AbilitySpec)
		{
			const FGameplayTag AbilityTag = FatedBrandASC->GetAbilityTagFromSpec(AbilitySpec);
			UDataAsset_AbilityInfo* AbilityInfo = UFatedBrandFunctionLibrary::GetAbilityInfo(this);
			FFatedBrandAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);

			FSavedAbility SavedAbility;
			SavedAbility.Ability = Info.Ability;
			SavedAbility.LevelRequirement = AbilitySpec.Level;
			SavedAbility.InputTag = FatedBrandASC->GetSlotFromAbilityTag(AbilityTag);
			SavedAbility.StatusTag = FatedBrandASC->GetStatusFromAbilityTag(AbilityTag);
			SavedAbility.AbilityTag = AbilityTag;
			SavedAbility.AbilityType = Info.AbilityType;

			SaveData->SavedAbilities.AddUnique(SavedAbility);
		});
		FatedBrandASC->ForEachAbility(SaveAbilityDelegate);

		FatedBrandGameMode->SaveInGameProgressData(SaveData);
	}
}

float AFatedBrandCharacter::GetVitalSurgeGage_Implementation()
{
	float CurrentValue = UFatedBrandAttributeSet::GetVitalSurgeAttribute().GetNumericValue(FatedBrandAttributeSet);
	if (CurrentValue > 0.f)
	{
		return CurrentValue;
	}
	return 0.f;
}

void AFatedBrandCharacter::InteractSavePoint_Implementation(const bool IsInteraction)
{
	if (PlayerController.IsValid()) PlayerController->SetIsCanCreateSaveMenu(IsInteraction);
}

void AFatedBrandCharacter::CanUpgradeAttribute_Implementation(const bool IsInteraction)
{
	if (PlayerController.IsValid()) PlayerController->SetIsCanCreateAttributeMenu(IsInteraction);
}

AFatedBrandPlayerState* AFatedBrandCharacter::GetPlayerState_Implementation()
{
	return PlayerController->GetPlayerState<AFatedBrandPlayerState>();
}

void AFatedBrandCharacter::LoadProgress()
{
	AddCharacterAbilities();
	if (const AFatedBrandGameModeBase* FatedBrandGameMode = Cast<AFatedBrandGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		UFatedBrandSaveGame* SaveData = FatedBrandGameMode->GetProgressSaveData();
		if (SaveData == nullptr) return;

		if (SaveData->bFirstTimeLoadIn)
		{
			//
		}
		else
		{
			if (UFatedBrandAbilitySystemComponent* FatedBrandASC = Cast<UFatedBrandAbilitySystemComponent>(GetAbilitySystemComponent()))
			{
				FatedBrandASC->AddCharacterAbilitiesFromSaveData(SaveData);
				UFatedBrandFunctionLibrary::InitializeDefaultAttributesFromSaveData(this, FatedBrandASC, SaveData);
			}
		}
	}
}

void AFatedBrandCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!PlayerController.IsValid()) PlayerController = Cast<AFatedBrandPlayerController>(NewController);

	InitAbilityActorInfo();
	LoadProgress();

	if (const AFatedBrandGameModeBase* FatedBrandGameMode = Cast<AFatedBrandGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		FatedBrandGameMode->LoadWorldSate(GetWorld());
	}
}

void AFatedBrandCharacter::InitAbilityActorInfo()
{
	if (AFatedBrandPlayerController* FatedBrandPlayerController = Cast<AFatedBrandPlayerController>(GetController()))
	{
		AFatedBrandPlayerState* FatedBrandPlayerState = GetPlayerState<AFatedBrandPlayerState>();
		check(FatedBrandPlayerState);

		FatedBrandPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(FatedBrandPlayerState, this);
		FatedBrandAbilitySystemComponent = FatedBrandPlayerState->GetAbilitySystemComponent();
		FatedBrandAttributeSet = FatedBrandPlayerState->GetAttributeSet();

		if (AFatedBrandHUD* FatedBrandHUD = Cast<AFatedBrandHUD>(FatedBrandPlayerController->GetHUD()))
		{
			FatedBrandHUD->InitOverlay(FatedBrandPlayerController, FatedBrandAbilitySystemComponent, FatedBrandAttributeSet);
		}
	}
}
