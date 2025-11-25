// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/FatedBrandInstance.h"

#include "GameFramework/GameUserSettings.h"

void UFatedBrandInstance::Init()
{
	Super::Init();

	if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
	{
		Settings->SetOverallScalabilityLevel(2);
		Settings->ApplySettings(false);
	}
}