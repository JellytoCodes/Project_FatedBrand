// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "FatedBrandStructTypes.h"
#include "GameFramework/PlayerState.h"
#include "FatedBrandPlayerState.generated.h"

struct FGameplayAbilitySpec;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class PROJECT_FATEDBRAND_API AFatedBrandPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public :
	AFatedBrandPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	UCurveTable* GetStatCurveTable() const { return StatCurveTable; }

	UFUNCTION(BlueprintCallable)
	void CaptureRunningCooldowns();

	UFUNCTION(BlueprintCallable)
	void ApplyRunningCooldowns();

protected :

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, Category = "Stat")
	TObjectPtr<UCurveTable> StatCurveTable;
};
