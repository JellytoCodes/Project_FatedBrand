// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/ViewModel/MVVM_LoadScreen.h"

#include "Game/FatedBrandGameModeBase.h"
#include "Game/FatedBrandInstance.h"
#include "HUD/ViewModel/MVVM_LoadSlot.h"
#include "Kismet/GameplayStatics.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->SetLoadSlotName("LoadSlot_0");
	LoadSlot_0->SlotIndex = 0;
	LoadSlots.Add(0, LoadSlot_0);

	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->SetLoadSlotName("LoadSlot_1");
	LoadSlot_1->SlotIndex = 1;
	LoadSlots.Add(1, LoadSlot_1);

	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2->SetLoadSlotName("LoadSlot_2");
	LoadSlot_2->SlotIndex = 2;
	LoadSlots.Add(2, LoadSlot_2);
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index)
{
	return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::SlotPressedForLoading(int32 Slot)
{
	AFatedBrandGameModeBase* FatedBrandGameMode = Cast<AFatedBrandGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(FatedBrandGameMode)) return;

	if (LoadSlots[Slot]->SlotStatus == NotValid)
	{
		LoadSlots[Slot]->SetMapName(FatedBrandGameMode->DefaultMapName);
		LoadSlots[Slot]->SlotStatus = Valid;
		LoadSlots[Slot]->PlayerStartTag = FatedBrandGameMode->DefaultPlayerStartTag;
		LoadSlots[Slot]->MapAssetName = FatedBrandGameMode->DefaultMap.ToSoftObjectPath().GetAssetName();
		FatedBrandGameMode->SaveSlotData(LoadSlots[Slot], Slot);

		UFatedBrandInstance* FatedBrandInstance = Cast<UFatedBrandInstance>(FatedBrandGameMode->GetGameInstance());
		FatedBrandInstance->LoadSlotName = LoadSlots[Slot]->GetLoadSlotName();
		FatedBrandInstance->LoadSlotIndex = LoadSlots[Slot]->SlotIndex;
		FatedBrandInstance->PlayerStartTag = LoadSlots[Slot]->PlayerStartTag;
	}

	SlotSelectedDelegate.Broadcast();
	for (const TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		if (LoadSlot.Key == Slot)	LoadSlot.Value->EnableSelectSlotDelegate.Broadcast(false);
		else						LoadSlot.Value->EnableSelectSlotDelegate.Broadcast(true);
	}
	SelectedSlot = LoadSlots[Slot];
}

void UMVVM_LoadScreen::SlotPressedForSaving(int32 Slot)
{
	// TODO : 세이브 포인트에서 사용으로 구현필요
}

void UMVVM_LoadScreen::SelectSlotPressedForPlay()
{
	if (!IsValid(SelectedSlot)) return;

	AFatedBrandGameModeBase* FatedBrandGameMode = Cast<AFatedBrandGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(FatedBrandGameMode)) return;

	UFatedBrandInstance* FatedBrandInstance = Cast<UFatedBrandInstance>(FatedBrandGameMode->GetGameInstance());
	FatedBrandInstance->PlayerStartTag = SelectedSlot->PlayerStartTag;
	FatedBrandInstance->LoadSlotIndex = SelectedSlot->SlotIndex;
	FatedBrandInstance->LoadSlotName = SelectedSlot->GetLoadSlotName();

	FatedBrandGameMode->TravelToMap(SelectedSlot);
}

void UMVVM_LoadScreen::LoadData()
{
	AFatedBrandGameModeBase* FatedBrandGameMode = Cast<AFatedBrandGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(FatedBrandGameMode)) return;

	for (const TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		UFatedBrandSaveGame* SaveObject = FatedBrandGameMode->GetSaveSlotData(LoadSlot.Value->GetLoadSlotName(), LoadSlot.Key);

		TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = SaveObject->SaveSlotStatus;

		LoadSlot.Value->SlotStatus = SaveSlotStatus;

		LoadSlot.Value->SetMapName(SaveObject->MapName);
		LoadSlot.Value->PlayerStartTag = SaveObject->PlayerStartTag;
	}
	
}