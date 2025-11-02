// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/FatedBrandCharacterBase.h"
#include "FatedBrandEnemy.generated.h"

class AFatedBrandAIController;
class UBehaviorTree;

UCLASS()
class PROJECT_FATEDBRAND_API AFatedBrandEnemy : public AFatedBrandCharacterBase
{
	GENERATED_BODY()

public :
	AFatedBrandEnemy();

#pragma region CombatInterface
	virtual void Die() override;
	virtual AActor* GetCombatTarget_Implementation() override;
#pragma endregion

protected :
	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AFatedBrandAIController> FatedBrandAIController;

};
