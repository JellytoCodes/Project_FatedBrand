// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "ActorInteractInterface.generated.h"

UINTERFACE(MinimalAPI)
class UActorInteractInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT_FATEDBRAND_API IActorInteractInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GiveAbilityToTarget(AActor* TargetActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FGameplayTag GetConditionTag();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetMatchesTagByTarget(AActor* TargetActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnHit();
};
