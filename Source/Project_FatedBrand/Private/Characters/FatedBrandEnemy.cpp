// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/FatedBrandEnemy.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Controllers/FatedBrandAIController.h"
#include "DataAssets/DataAsset_StartUpDataBase.h"
#include "Engine/AssetManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Project_FatedBrand/Project_FatedBrand.h"

AFatedBrandEnemy::AFatedBrandEnemy()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void AFatedBrandEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!StartUpData.IsNull())
	{
		UAssetManager::GetStreamableManager().RequestAsyncLoad(StartUpData.ToSoftObjectPath(), 
		FStreamableDelegate::CreateLambda([this]()
		{
			if (UDataAsset_StartUpDataBase* LoadedData = StartUpData.LoadSynchronous())
			{
				LoadedData->InitializeGameplayEffect(FatedBrandAbilitySystemComponent, StartUpCharacterName, 1);
			}	
		}));
	}
	if (BehaviorTree)
	{
		FatedBrandAIController = Cast<AFatedBrandAIController>(NewController);
		FatedBrandAIController->RunBehaviorTree(BehaviorTree);	
	}
}
