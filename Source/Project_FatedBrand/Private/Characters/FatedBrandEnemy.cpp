// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/FatedBrandEnemy.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Controllers/FatedBrandAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Project_FatedBrand/Project_FatedBrand.h"

AFatedBrandEnemy::AFatedBrandEnemy()
{
	GetMesh()->SetCollisionObjectType(ECC_Enemy);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void AFatedBrandEnemy::Die()
{
	FatedBrandAIController->GetBlackboardComponent()->SetValueAsBool("IsDeath", true);

	Super::Die();
}

AActor* AFatedBrandEnemy::GetCombatTarget_Implementation()
{
	if (FatedBrandAIController->GetBlackboardComponent()->GetValueAsObject("TargetActor") == nullptr) return nullptr;
	return Cast<AActor>(FatedBrandAIController->GetBlackboardComponent()->GetValueAsObject("TargetActor"));
}

void AFatedBrandEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (BehaviorTree)
	{
		FatedBrandAIController = Cast<AFatedBrandAIController>(NewController);
		FatedBrandAIController->RunBehaviorTree(BehaviorTree);	
	}
}

void AFatedBrandEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (FatedBrandAbilitySystemComponent)
	{
		AddCharacterAbilities();	
	}
}
