// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DataAssets/DataAsset_InputConfig.h"
#include "FatedBrandEnhancedInputComponent.generated.h"

UCLASS()
class PROJECT_FATEDBRAND_API UFatedBrandEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public :
	template<class UserObject, typename CallbackFunc>
	void BindNativeInputAction(UDataAsset_InputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func);

	template<class UserObject, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityInputAction(UDataAsset_InputConfig* InInputConfig, UserObject* ContextObject, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template <class UserObject, typename CallbackFunc>
void UFatedBrandEnhancedInputComponent::BindNativeInputAction(UDataAsset_InputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func)
{
	check(InInputConfig);

	if (UInputAction* FoundAction = InInputConfig->FindNativeInputActionByTag(InInputTag))
	{
		BindAction(FoundAction, TriggerEvent, ContextObject, Func);
	}
}

template <class UserObject, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UFatedBrandEnhancedInputComponent::BindAbilityInputAction(UDataAsset_InputConfig* InInputConfig, UserObject* ContextObject,  PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InInputConfig);

	for (const FFatedBrandInputAction& AbilityInputConfig : InInputConfig->AbilityInputActions)
	{
		if (AbilityInputConfig.IsValid())
		{
			if (PressedFunc)	BindAction(AbilityInputConfig.InputAction, ETriggerEvent::Started, ContextObject, PressedFunc, AbilityInputConfig.InputTag);
			if (ReleasedFunc)	BindAction(AbilityInputConfig.InputAction, ETriggerEvent::Completed, ContextObject, ReleasedFunc, AbilityInputConfig.InputTag);
			//if (HeldFunc)		BindAction(AbilityInputConfig.InputAction, ETriggerEvent::Triggered, ContextObject, HeldFunc, AbilityInputConfig.InputTag);
		}
	}
}
