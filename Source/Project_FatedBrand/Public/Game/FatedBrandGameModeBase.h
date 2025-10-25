// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FatedBrandGameModeBase.generated.h"

class UDataAsset_AbilityInfo;

UCLASS()
class PROJECT_FATEDBRAND_API AFatedBrandGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public :
	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TObjectPtr<UDataAsset_AbilityInfo> AbilityInfo;

protected :
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
};
