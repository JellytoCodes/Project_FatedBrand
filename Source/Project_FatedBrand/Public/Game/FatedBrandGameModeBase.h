// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FatedBrandGameModeBase.generated.h"

class UMVVM_LoadSlot;
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

	void LoadWorldSate(UWorld* World) const;

	FString GetMapNameFromMapAssetName(const FString& MapAssetName) const;

	void SaveSlotData(UMVVM_LoadSlot* LoadSlot, const int32 SlotIndex);

	UFatedBrandSaveGame* RetrieveInGameSaveData() const;
	UFatedBrandSaveGame* GetSaveSlotData(const FString& SlotName, int SlotIndex) const;

	void TravelToMap(const UMVVM_LoadSlot* Slot);

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

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	/** 리팩토링 진행 중 데이터 */

	void SaveProgressWorldState(UWorld* World, const FString& DestinationMapAssetName = FString("")) const;
	void SaveInGameProgressData(UFatedBrandSaveGame* SaveObject) const;
	UFatedBrandSaveGame* GetProgressSaveData() const;

	UFUNCTION(BlueprintCallable)
	void ProgressSaveDataToSlot(const FString& SlotName, const int SlotIndex);

	FString ProgressSlotName = "ProgressSaveSlot";
	int32 ProgressSlotIndex = 99;

	/** */

protected :
	virtual void BeginPlay() override;
};
