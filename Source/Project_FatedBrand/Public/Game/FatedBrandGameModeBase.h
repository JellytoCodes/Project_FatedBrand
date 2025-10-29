// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FatedBrandGameModeBase.generated.h"

class USaveGame;
class UFatedBrandSaveGame;
class UDataAsset_AbilityInfo;

UCLASS()
class PROJECT_FATEDBRAND_API AFatedBrandGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public :
	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TObjectPtr<UDataAsset_AbilityInfo> AbilityInfo;

	void SaveWorldState(UWorld* World, const FString& DestinationMapAssetName = FString("")) const;
	void LoadWorldSate(UWorld* World) const;

	UFatedBrandSaveGame* RetrieveInGameSaveData() const;
	UFatedBrandSaveGame* GetSaveSlotData(const FString& SlotName, int SlotIndex) const;

	void SaveInGameProgressData(UFatedBrandSaveGame* SaveObject) const;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> SaveGameClass;

	UPROPERTY(EditDefaultsOnly)
	FString DefaultMapName;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DefaultMap;

	UPROPERTY(EditDefaultsOnly)
	FName DefaultPlayerStartTag;

	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;

	FString GetMapNameFromMapAssetName(const FString& MapAssetName) const;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

protected :
	virtual void BeginPlay() override;
};
