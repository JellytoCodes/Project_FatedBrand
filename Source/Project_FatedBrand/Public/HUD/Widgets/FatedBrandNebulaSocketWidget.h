// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/Widgets/FatedBrandUserWidget.h"
#include "FatedBrandNebulaSocketWidget.generated.h"

class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSelectSocketSignature);

UCLASS()
class PROJECT_FATEDBRAND_API UFatedBrandNebulaSocketWidget : public UFatedBrandUserWidget
{
	GENERATED_BODY()

public :
	UImage* GetSelectImage() const { return Select_Image; }

	UPROPERTY(BlueprintAssignable)
	FOnSelectSocketSignature OnSelectSocketDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	FGameplayTag AbilityTag;

private :
	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UImage> Select_Image;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UImage> Skill_Image;
};
