// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/Widgets/FatedBrandUserWidget.h"
#include "FatedBrandStructTypes.h"
#include "FatedBrandNebulaMenuWidget.generated.h"

class UTextBlock;
class UFatedBrandHotBarWidget;
class UFatedBrandNebulaSocketWidget;

UCLASS()
class PROJECT_FATEDBRAND_API UFatedBrandNebulaMenuWidget : public UFatedBrandUserWidget
{
	GENERATED_BODY()

public :

	UFUNCTION(BlueprintCallable)
	void SetNebulaDescription(FString InDescription);

private :

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TSoftObjectPtr<UTextBlock> NebulaDescription;
};
