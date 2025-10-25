// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FatedBrandInteractActor.generated.h"

UCLASS()
class PROJECT_FATEDBRAND_API AFatedBrandInteractActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AFatedBrandInteractActor();

protected:
	virtual void BeginPlay() override;

};
