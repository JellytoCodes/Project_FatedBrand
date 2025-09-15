// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FatedBrandWidgetController.generated.h"

struct FFatedBrandAbilityInfo;
class UFatedBrandAttributeSet;
class UFatedBrandAbilitySystemComponent;
class AFatedBrandPlayerController;
class UDataAsset_AbilityInfo;
class UAttributeSet;
class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FFatedBrandAbilityInfo&, Info);

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}

	FWidgetControllerParams(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS)
		: PlayerController(PC), AbilitySystemComponent(ASC), AttributeSet(AS) {}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

UCLASS()
class PROJECT_FATEDBRAND_API UFatedBrandWidgetController : public UObject
{
	GENERATED_BODY()

public :
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues() {}
	virtual void BindCallbacksToDependencies() {}

	UPROPERTY(BlueprintAssignable, Category = "GAS|Message")
	FAbilityInfoSignature AbilityInfoDelegate;

	void BroadcastAbilityInfo();

protected :
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataAsset_AbilityInfo> AbilityInfo;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<AFatedBrandPlayerController> FatedBrandPlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UFatedBrandAbilitySystemComponent> FatedBrandAbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UFatedBrandAttributeSet> FatedBrandAttributeSet;

	AFatedBrandPlayerController* GetFatedBrandPC();
	UFatedBrandAbilitySystemComponent* GetFatedBrandASC();
	UFatedBrandAttributeSet* GetFatedBrandAS();
};
