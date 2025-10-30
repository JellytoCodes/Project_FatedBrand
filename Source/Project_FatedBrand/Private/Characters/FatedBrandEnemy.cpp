// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/FatedBrandEnemy.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Controllers/FatedBrandAIController.h"
#include "DataAssets/DataAsset_StartUpDataBase.h"
#include "Engine/AssetManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystem/FatedBrandAbilitySystemComponent.h"

AFatedBrandEnemy::AFatedBrandEnemy()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	FatedBrandAbilitySystemComponent->SetIsReplicated(true);
	FatedBrandAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void AFatedBrandEnemy::Die()
{
	FatedBrandAIController->GetBlackboardComponent()->SetValueAsBool("IsDeath", true);

	Super::Die();
}

void AFatedBrandEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AddCharacterAbilities();

	if (BehaviorTree)
	{
		FatedBrandAIController = Cast<AFatedBrandAIController>(NewController);
		FatedBrandAIController->RunBehaviorTree(BehaviorTree);	
	}
}
