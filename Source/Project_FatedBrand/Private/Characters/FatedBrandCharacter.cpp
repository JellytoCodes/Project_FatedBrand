// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/FatedBrandCharacter.h"

#include "AbilitySystem/FatedBrandAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Controllers/FatedBrandPlayerController.h"
#include "DataAssets/DataAsset_StartUpDataBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "HUD/FatedBrandHUD.h"

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

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 4000.f, 0.f);
}

void AFatedBrandCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!StartUpData.IsNull())
	{
		if (UDataAsset_StartUpDataBase* LoadedData = StartUpData.LoadSynchronous())
		{
			LoadedData->InitializeGameplayEffect(FatedBrandAbilitySystemComponent, StartUpCharacterName, 1);
		}
	}
	InitAbilityActorInfo();
}

void AFatedBrandCharacter::InitAbilityActorInfo()
{
	if (AFatedBrandPlayerController* FatedBrandPlayerController = Cast<AFatedBrandPlayerController>(GetController()))
	{
		if (AFatedBrandHUD* FatedBrandHUD = Cast<AFatedBrandHUD>(FatedBrandPlayerController->GetHUD()))
		{
			FatedBrandHUD->InitOverlay(FatedBrandPlayerController, GetAbilitySystemComponent(), FatedBrandAttributeSet);
		}
	}

	//캐릭터 기본 소유 액티브/패시브 스킬 ASC에 등록
	GetFatedBrandAbilitySystemComponent()->AddCharacterActivateAbilities(StartUpOffensiveAbilities);
	GetFatedBrandAbilitySystemComponent()->AddCharacterPassiveAbilities(StartUpPassiveAbilities);
}
