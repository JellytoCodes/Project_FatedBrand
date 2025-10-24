// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/Widgets/FatedBrandUserWidget.h"
#include "FatedBrandHotBarWidget.generated.h"

class UImage;

UCLASS()
class PROJECT_FATEDBRAND_API UFatedBrandHotBarWidget : public UFatedBrandUserWidget
{
	GENERATED_BODY()

public :
	UImage* GetSelectImage() const { return Select_Image; }

private :
	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UImage> Select_Image;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UImage> Skill_Image;
};
