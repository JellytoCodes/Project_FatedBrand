// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/FatedBrandGameModeBase.h"

#include "EngineUtils.h"
#include "Game/FatedBrandInstance.h"
#include "Game/FatedBrandSaveGame.h"
#include "Interfaces/SaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Project_FatedBrand/Project_FatedBrand.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

UFatedBrandSaveGame* AFatedBrandGameModeBase::GetSaveSlotData(const FString& SlotName, int SlotIndex) const
{
	USaveGame* SaveGameObject = UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex) ?
		UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex) : UGameplayStatics::CreateSaveGameObject(SaveGameClass);

	UFatedBrandSaveGame* LoadSaveGame = Cast<UFatedBrandSaveGame>(SaveGameObject);
	return LoadSaveGame;
}

void AFatedBrandGameModeBase::SaveInGameProgressData(UFatedBrandSaveGame* SaveObject) const
{
	UFatedBrandInstance* FatedBrandInstance = Cast<UFatedBrandInstance>(GetGameInstance());

	const FString InGameLoadSlotName = FatedBrandInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = FatedBrandInstance->LoadSlotIndex;
	FatedBrandInstance->PlayerStartTag = SaveObject->PlayerStartTag;

	// 테스트용으로 슬롯 네임 강제화
	UGameplayStatics::SaveGameToSlot(SaveObject, FString("SaveSlot"), InGameLoadSlotIndex);
}

void AFatedBrandGameModeBase::SaveWorldState(UWorld* World, const FString& DestinationMapAssetName) const
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	const UFatedBrandInstance* FatedBrandInstance = Cast<UFatedBrandInstance>(GetGameInstance());

	if (UFatedBrandSaveGame* SaveGame = GetSaveSlotData(FatedBrandInstance->LoadSlotName, FatedBrandInstance->LoadSlotIndex))
	{
		if (DestinationMapAssetName != FString(""))
		{
			SaveGame->MapAssetName = DestinationMapAssetName;
			SaveGame->MapName = GetMapNameFromMapAssetName(DestinationMapAssetName);
		}
		if (!SaveGame->HasMap(WorldName))
		{
			FSavedMap NewSavedMap;
			NewSavedMap.MapAssetName = WorldName;
			SaveGame->SavedMaps.Add(NewSavedMap);
		}

		FSavedMap SavedMap = SaveGame->GetSavedMapWithMapName(WorldName);
		SavedMap.SavedActors.Empty();

		for (FActorIterator It(World) ; It ; ++It)
		{
			AActor* Actor = *It;
			if (!IsValid(Actor) || !Actor->Implements<USaveInterface>()) continue;

			FSavedActor SavedActor;
			SavedActor.ActorName = Actor->GetFName();
			SavedActor.Transform = Actor->GetTransform();

			FMemoryWriter MemoryWrite(SavedActor.Bytes);

			FObjectAndNameAsStringProxyArchive Archive(MemoryWrite, true);

			Actor->Serialize(Archive);
			SavedMap.SavedActors.AddUnique(SavedActor);
		}

		for (FSavedMap& MapToReplace : SaveGame->SavedMaps)
		{
			if (MapToReplace.MapAssetName == WorldName)
			{
				MapToReplace = SavedMap;
				break;
			}
		}
		UGameplayStatics::SaveGameToSlot(SaveGame, FatedBrandInstance->LoadSlotName, FatedBrandInstance->LoadSlotIndex);
	}
}

void AFatedBrandGameModeBase::LoadWorldSate(UWorld* World) const
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	const UFatedBrandInstance* FatedBrandInstance = Cast<UFatedBrandInstance>(GetGameInstance());
	check(FatedBrandInstance);

	if (UGameplayStatics::DoesSaveGameExist(FatedBrandInstance->LoadSlotName, FatedBrandInstance->LoadSlotIndex))
	{
		UFatedBrandSaveGame* SaveGame = Cast<UFatedBrandSaveGame>(UGameplayStatics::LoadGameFromSlot(FatedBrandInstance->LoadSlotName, FatedBrandInstance->LoadSlotIndex));
		if (SaveGame == nullptr) return;

		for (FActorIterator It(World) ; It ; ++It)
		{
			AActor* Actor = *It;

			if (!Actor->Implements<USaveInterface>()) continue;

			for (FSavedActor SavedActor : SaveGame->GetSavedMapWithMapName(WorldName).SavedActors)
			{
				if (SavedActor.ActorName == Actor->GetName())
				{
					if (ISaveInterface::Execute_ShouldLoadTransform(Actor))
					{
						Actor->SetActorTransform(SavedActor.Transform);
					}
					FMemoryReader MemoryReader(SavedActor.Bytes);

					FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
					Archive.ArIsSaveGame = true;
					Actor->Serialize(Archive);

					ISaveInterface::Execute_LoadActor(Actor);
				}
			}
		}
	}
}

UFatedBrandSaveGame* AFatedBrandGameModeBase::RetrieveInGameSaveData() const
{
	const UFatedBrandInstance* FatedBrandInstance = Cast<UFatedBrandInstance>(GetGameInstance());

	const FString InGameLoadSlotName = FatedBrandInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = FatedBrandInstance->LoadSlotIndex;

	// 테스트용으로 슬롯 네임 강제화
	return GetSaveSlotData(FString("SaveSlot"), InGameLoadSlotIndex);
}

AActor* AFatedBrandGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	return Super::ChoosePlayerStart_Implementation(Player);

}

FString AFatedBrandGameModeBase::GetMapNameFromMapAssetName(const FString& MapAssetName) const
{
	for (auto& Map : Maps)
	{
		if (Map.Value.ToSoftObjectPath().GetAssetName() == MapAssetName)
		{
			return Map.Key;
		}
	}
	return FString();
}

void AFatedBrandGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}
