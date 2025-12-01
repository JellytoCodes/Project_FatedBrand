// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/FatedBrandInstance.h"

#include "GameFramework/GameUserSettings.h"

void UFatedBrandInstance::Init()
{
	Super::Init();

	if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
	{
		Settings->SetOverallScalabilityLevel(ScalabilityLevel);
		Settings->ApplySettings(false);
	}
}

void UFatedBrandInstance::SetScalabilityLevel(const int32 Value)
{
	if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
	{
		Settings->SetOverallScalabilityLevel(Value);
		Settings->ApplySettings(false);
	}
}
