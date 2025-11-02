// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/FatedBrandGameModeBase.h"

#include "EngineUtils.h"
#include "Game/FatedBrandInstance.h"
#include "Game/FatedBrandSaveGame.h"
#include "GameFramework/PlayerStart.h"
#include "HUD/ViewModel/MVVM_LoadSlot.h"
#include "Interfaces/SaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Project_FatedBrand/Project_FatedBrand.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

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

void AFatedBrandGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, const int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->GetLoadSlotName(), SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->GetLoadSlotName(), SlotIndex);
	}

	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(SaveGameClass);
	UFatedBrandSaveGame* LoadSaveGame = Cast<UFatedBrandSaveGame>(SaveGameObject);
	LoadSaveGame->SaveSlotStatus = Valid;
	LoadSaveGame->MapName = LoadSlot->GetMapName();
	LoadSaveGame->MapAssetName = LoadSlot->MapAssetName;
	LoadSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;

	UGameplayStatics::SaveGameToSlot(LoadSaveGame, LoadSlot->GetLoadSlotName(), SlotIndex);
}

UFatedBrandSaveGame* AFatedBrandGameModeBase::GetSaveSlotData(const FString& SlotName, int SlotIndex) const
{
	USaveGame* SaveGameObject = UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex) ?
		UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex) : UGameplayStatics::CreateSaveGameObject(SaveGameClass);

	UFatedBrandSaveGame* LoadSaveGame = Cast<UFatedBrandSaveGame>(SaveGameObject);
	return LoadSaveGame;
}

UFatedBrandSaveGame* AFatedBrandGameModeBase::RetrieveInGameSaveData() const
{
	const UFatedBrandInstance* FatedBrandInstance = Cast<UFatedBrandInstance>(GetGameInstance());

	const FString InGameLoadSlotName = FatedBrandInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = FatedBrandInstance->LoadSlotIndex;

	return GetSaveSlotData(InGameLoadSlotName, InGameLoadSlotIndex);
}

void AFatedBrandGameModeBase::SaveInGameProgressData(UFatedBrandSaveGame* SaveObject) const
{
	UFatedBrandInstance* FatedBrandInstance = Cast<UFatedBrandInstance>(GetGameInstance());

	FatedBrandInstance->PlayerStartTag = SaveObject->PlayerStartTag;

	UGameplayStatics::SaveGameToSlot(SaveObject, ProgressSlotName, ProgressSlotIndex);
}

void AFatedBrandGameModeBase::TravelToMap(const UMVVM_LoadSlot* Slot)
{
	const FString SlotName = Slot->GetLoadSlotName();
	const int32 SlotIndex = Slot->SlotIndex;

	UGameplayStatics::OpenLevelBySoftObjectPtr(Slot, Maps.FindChecked(Slot->GetMapName()));
}

AActor* AFatedBrandGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<AActor*> Actors;
	UFatedBrandInstance* FatedBrandInstance = Cast<UFatedBrandInstance>(GetGameInstance());
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
	if (Actors.Num() <= 0) return nullptr;

	AActor* SelectedActor = Actors[0];
	for (AActor* Actor : Actors)
	{
		if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
		{
			if (PlayerStart->PlayerStartTag == FatedBrandInstance->PlayerStartTag)
			{
				SelectedActor = PlayerStart;
				break;
			}
		}
	}
	return SelectedActor;
}

void AFatedBrandGameModeBase::SaveProgressWorldState(UWorld* World, const FString& DestinationMapAssetName) const
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	const UFatedBrandInstance* FatedBrandInstance = Cast<UFatedBrandInstance>(GetGameInstance());

	if (UFatedBrandSaveGame* SaveGame = GetProgressSaveData())
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
			Archive.ArIsSaveGame = true;

			Actor->Serialize(Archive);
			SavedMap.SavedActors.AddUnique(SavedActor);
		}

		for (FSavedMap& MapToReplace : SaveGame->SavedMaps)
		{
			if (MapToReplace.MapAssetName == WorldName)
			{
				MapToReplace = SavedMap;
			}
		}
		UGameplayStatics::SaveGameToSlot(SaveGame, ProgressSlotName, ProgressSlotIndex);
	}
}

UFatedBrandSaveGame* AFatedBrandGameModeBase::GetProgressSaveData() const
{
	USaveGame* SaveGameObject = UGameplayStatics::DoesSaveGameExist(ProgressSlotName, ProgressSlotIndex) ?
		UGameplayStatics::LoadGameFromSlot(ProgressSlotName, ProgressSlotIndex) : UGameplayStatics::CreateSaveGameObject(SaveGameClass);

	UFatedBrandSaveGame* LoadSaveGame = Cast<UFatedBrandSaveGame>(SaveGameObject);

	return LoadSaveGame;
}

void AFatedBrandGameModeBase::ProgressSaveDataToSlot(const FString& SlotName, const int SlotIndex)
{
	if (UFatedBrandSaveGame* ProgressSaveData = GetProgressSaveData())
	{
		ProgressSaveData->SaveSlotStatus = Valid;
		UGameplayStatics::SaveGameToSlot(ProgressSaveData, SlotName, SlotIndex);
	}
}

void AFatedBrandGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Maps.Add(DefaultMapName, DefaultMap);
}
