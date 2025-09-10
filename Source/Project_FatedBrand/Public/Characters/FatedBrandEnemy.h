// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/FatedBrandCharacterBase.h"
#include "FatedBrandEnemy.generated.h"

UCLASS()
class PROJECT_FATEDBRAND_API AFatedBrandEnemy : public AFatedBrandCharacterBase
{
	GENERATED_BODY()

public :
	AFatedBrandEnemy();

protected :
	virtual void PossessedBy(AController* NewController) override;

};
