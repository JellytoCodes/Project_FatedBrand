// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/FatedBrandEnemy.h"

#include "DataAssets/DataAsset_StartUpDataBase.h"
#include "Engine/AssetManager.h"
#include "Project_FatedBrand/Project_FatedBrand.h"

AFatedBrandEnemy::AFatedBrandEnemy()
{

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
				LoadedData->GiveToAbilitySystemComponent(FatedBrandAbilitySystemComponent, 1);
			}	
		}));
	}
}
