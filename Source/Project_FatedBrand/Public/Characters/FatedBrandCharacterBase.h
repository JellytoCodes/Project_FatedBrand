// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "DataAssets/DataAsset_StartUpDataBase.h"
#include "GameFramework/Character.h"
#include "FatedBrandCharacterBase.generated.h"

class UFatedBrandAttributeSet;
class UFatedBrandAbilitySystemComponent;
class UAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponEquippedStatusSignature, bool, bIsEquip);

UCLASS()
class PROJECT_FATEDBRAND_API AFatedBrandCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AFatedBrandCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	FORCEINLINE UFatedBrandAbilitySystemComponent* GetFatedBrandAbilitySystemComponent() const { return FatedBrandAbilitySystemComponent; }
	FORCEINLINE UAttributeSet* GetFatedBrandAttributeSet() const { return FatedBrandAttributeSet; }

	UPROPERTY(BlueprintAssignable, Category = "AbilitySystem")
	FWeaponEquippedStatusSignature WeaponEquippedStatusDelegate;

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void SendWeaponEquippedDelegate(const bool bIsEquip) const;

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void ToggleCurrentEquippedWeapon(const bool bShouldEnable);

protected:
	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<UStaticMeshComponent> Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	TObjectPtr<UFatedBrandAbilitySystemComponent> FatedBrandAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	TObjectPtr<UAttributeSet> FatedBrandAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	EStartUpCharacterName StartUpCharacterName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	TSoftObjectPtr<UDataAsset_StartUpDataBase> StartUpData;

	virtual void InitAbilityActorInfo() { }

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
