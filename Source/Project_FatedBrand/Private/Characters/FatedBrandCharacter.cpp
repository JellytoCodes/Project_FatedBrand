// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/FatedBrandCharacter.h"

#include "FatedBrandFunctionLibrary.h"
#include "AbilitySystem/FatedBrandAbilitySystemComponent.h"
#include "AbilitySystem/FatedBrandAttributeSet.h"
#include "Controllers/FatedBrandPlayerController.h"
#include "DataAssets/DataAsset_AbilityInfo.h"
#include "Game/FatedBrandGameModeBase.h"
#include "Game/FatedBrandInstance.h"
#include "Game/FatedBrandPlayerState.h"
#include "Game/FatedBrandSaveGame.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/FatedBrandHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Project_FatedBrand/Project_FatedBrand.h"

AFatedBrandCharacter::AFatedBrandCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

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
	if (const AFatedBrandGameModeBase* FatedBrandGameMode = Cast<AFatedBrandGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		UFatedBrandSaveGame* SaveData = FatedBrandGameMode->GetProgressSaveData();
		if (SaveData == nullptr) return;

		SaveData->PlayerStartTag = CheckPointTag;

		SaveData->Vigor = UFatedBrandAttributeSet::GetVigorAttribute().GetNumericValue(FatedBrandAttributeSet);
		SaveData->CurrentHealth = UFatedBrandAttributeSet::GetCurrentHealthAttribute().GetNumericValue(FatedBrandAttributeSet);
		SaveData->Strength = UFatedBrandAttributeSet::GetStrengthAttribute().GetNumericValue(FatedBrandAttributeSet);
		SaveData->VitalSurge = UFatedBrandAttributeSet::GetVitalSurgeAttribute().GetNumericValue(FatedBrandAttributeSet);
		SaveData->EnhancedCore = UFatedBrandAttributeSet::GetEnhancedCoreAttribute().GetNumericValue(FatedBrandAttributeSet);

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
	const float CurrentValue = UFatedBrandAttributeSet::GetVitalSurgeAttribute().GetNumericValue(FatedBrandAttributeSet);

	return CurrentValue > 0.f ? CurrentValue : 0.f;
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

void AFatedBrandCharacter::PlayerHideHUD_Implementation()
{
	if (PlayerController.IsValid())
	{
		// Player Overlay를 제거하는 상황은 대부분
		// 컨트롤 권한을 잃는 경우가 많기에 MappingContext도 해제.
		PlayerController->DisableDefaultMappingContext();
		PlayerController->DisableHUD();
	}
}

void AFatedBrandCharacter::ChangeToTravelState_Implementation()
{
	if (UFatedBrandInstance* FatedBrandInstance = Cast<UFatedBrandInstance>(GetGameInstance()))
	{
		FatedBrandInstance->IsPlayerTravelTrigger = true;
	}
}

bool AFatedBrandCharacter::CanRopeSwing_Implementation(const bool IsCanGrab)
{
	IsHanging = IsCanGrab;
	return IsHanging;
}

void AFatedBrandCharacter::SetCanRest_Implementation(const bool InCanRest, AActor* InActor)
{
	CanShit = InCanRest;
	InteractActor = InActor;
}

void AFatedBrandCharacter::LoadProgress()
{
	AddCharacterAbilities();
	if (const AFatedBrandGameModeBase* FatedBrandGameMode = Cast<AFatedBrandGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		UFatedBrandInstance* FatedBrandInstance = Cast<UFatedBrandInstance>(GetGameInstance());
		if (FatedBrandInstance == nullptr) return;
		
		UFatedBrandSaveGame* SaveData = FatedBrandInstance->IsPlayerTravelTrigger ? FatedBrandGameMode->GetProgressSaveData() : FatedBrandGameMode->RetrieveInGameSaveData();
		if (SaveData == nullptr) return;

		if (SaveData->bFirstTimeLoadIn)
		{
			// 현재 데이터가 첫 시작일 경우 로드하고자 하는 데이터가 있을 때 사용
			// 아직은 기능 추가할 경우가 없어 공백 유지
		}
		else
		{
			if (UFatedBrandAbilitySystemComponent* FatedBrandASC = Cast<UFatedBrandAbilitySystemComponent>(GetAbilitySystemComponent()))
			{
				FatedBrandASC->AddCharacterAbilitiesFromSaveData(SaveData);
				UFatedBrandFunctionLibrary::InitializeDefaultAttributesFromSaveData(this, FatedBrandASC, SaveData);
			}
		}
		// 경우에 상관없이 무조건적으로 false로 처리하여
		// 재시작 발생 시 불러오는 데이터가 꼬이는 것을 방지하기 위함.
		FatedBrandInstance->IsPlayerTravelTrigger = false;
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

		AFatedBrandPlayerState* FatedBrandPlayerState = GetPlayerState<AFatedBrandPlayerState>();
		check(FatedBrandPlayerState);

		FatedBrandPlayerState->ApplyRunningCooldowns();
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