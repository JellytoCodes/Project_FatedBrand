// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/FatedBrandCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "FatedBrandFunctionLibrary.h"
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

	GetCharacterMovement()->PerchRadiusThreshold = 15.0f;
	GetCharacterMovement()->LedgeCheckThreshold = 6.0f;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 4000.f, 0.f);

	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, 1.0f, 0.0f));
	GetCharacterMovement()->bConstrainToPlane = true;
}

void AFatedBrandCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
}

void AFatedBrandCharacter::InitAbilityActorInfo()
{
	if (!StartUpData.IsNull())
	{
		if (UDataAsset_StartUpDataBase* LoadedData = StartUpData.LoadSynchronous())
		{
			LoadedData->InitializeGameplayEffect(FatedBrandAbilitySystemComponent, StartUpCharacterName, 1);

			//캐릭터 기본 소유 액티브/패시브 스킬 ASC에 등록
			GetFatedBrandAbilitySystemComponent()->AddCharacterActivateAbilities(LoadedData->StartUpOffensiveAbilities);
			GetFatedBrandAbilitySystemComponent()->AddCharacterPassiveAbilities(LoadedData->StartUpPassiveAbilities);
		}
	}

	if (AFatedBrandPlayerController* FatedBrandPlayerController = Cast<AFatedBrandPlayerController>(GetController()))
	{
		if (AFatedBrandHUD* FatedBrandHUD = Cast<AFatedBrandHUD>(FatedBrandPlayerController->GetHUD()))
		{
			FatedBrandHUD->InitOverlay(FatedBrandPlayerController, GetAbilitySystemComponent(), FatedBrandAttributeSet);
		}
	}
}

void AFatedBrandCharacter::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) return;

	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		SwordDamageEffectParams.TargetAbilitySystemComponent = TargetASC;
		UFatedBrandFunctionLibrary::ApplyDamageEffect(SwordDamageEffectParams);
	}
}
