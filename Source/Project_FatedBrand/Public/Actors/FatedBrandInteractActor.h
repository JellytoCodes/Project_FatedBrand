// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ActorInteractInterface.h"
#include "FatedBrandInteractActor.generated.h"

UCLASS()
class PROJECT_FATEDBRAND_API AFatedBrandInteractActor : public AActor, public IActorInteractInterface
{
	GENERATED_BODY()
	
public:	
	AFatedBrandInteractActor();

	UFUNCTION(BlueprintCallable)
	virtual void GiveAbilityToTarget(AActor* TargetActor) override;

private :
	UPROPERTY(EditAnywhere, Category = "Ability")
	FGameplayTag AbilityTag;
};
