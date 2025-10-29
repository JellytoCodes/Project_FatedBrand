// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/FatedBrandSaveGame.h"

FSavedMap UFatedBrandSaveGame::GetSavedMapWithMapName(const FString& InMapName)
{
	for (FSavedMap& Map : SavedMaps)
	{
		if (Map.MapAssetName == InMapName) return Map;
	}
	return FSavedMap();
}

bool UFatedBrandSaveGame::HasMap(const FString& InMapName)
{
	for (FSavedMap& Map : SavedMaps)
	{
		if (Map.MapAssetName == InMapName) return true;
	}
	return false;
}
