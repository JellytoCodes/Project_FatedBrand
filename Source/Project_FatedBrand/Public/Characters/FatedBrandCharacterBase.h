// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "FatedBrandStructTypes.h"
#include "DataAssets/DataAsset_StartUpDataBase.h"
#include "GameFramework/Character.h"
#include "Interfaces/CombatInterface.h"
#include "FatedBrandCharacterBase.generated.h"

class UBoxComponent;

UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
	EquippedWeapon,
	LeftHand,
	RightHand
};

class UFatedBrandAttributeSet;
class UFatedBrandAbilitySystemComponent;
class UAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponEquippedStatusSignature, bool, bIsEquip);

UCLASS()
class PROJECT_FATEDBRAND_API AFatedBrandCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AFatedBrandCharacterBase();

	UFUNCTION(BlueprintPure)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintPure)
	FORCEINLINE UFatedBrandAbilitySystemComponent* GetFatedBrandAbilitySystemComponent() const { return FatedBrandAbilitySystemComponent; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE UAttributeSet* GetFatedBrandAttributeSet() const { return FatedBrandAttributeSet; }

	// -----------------------------
	UPROPERTY(BlueprintAssignable, Category = "AbilitySystem")
	FWeaponEquippedStatusSignature WeaponEquippedStatusDelegate;

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void SendWeaponEquippedDelegate(const bool bIsEquip) const;
	// ~ Use Weapon Dissolve Material

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void ToggleCurrentCollision(const bool bShouldEnable, const EToggleDamageType ToggleDamageType = EToggleDamageType::EquippedWeapon);

	UPROPERTY(BlueprintReadWrite)
	FDamageEffectParams CombatDamageEffectParams;
	
#pragma region CombatInterface
	virtual void Die() override;

	virtual AActor* GetAvatar_Implementation() override;
	virtual bool IsDead_Implementation() const override;

	UPROPERTY(BlueprintAssignable, Category = "CombatInterface|Delegate")
	FOnDeathSignature OnDeathDelegate;
#pragma endregion

protected:
	virtual void PossessedBy(AController* NewController) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

#pragma region CombatCollision
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Weapon")
	TObjectPtr<UStaticMeshComponent> Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Body")
	TObjectPtr<UBoxComponent> LeftHandCollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Body")
	TObjectPtr<UBoxComponent> RightHandCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Body")
	FName LeftHandCollisionBoxAttachBoneName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Body")
	FName RightHandCollisionBoxAttachBoneName;
	
	void SetToggleCollisionEnabled(const EToggleDamageType ToggleDamageType, const ECollisionEnabled::Type CurrentCollisionType) const;
	void OnHitTargetActor(AActor* HitActor);

	UFUNCTION()
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	EToggleDamageType CurrentDamageType = EToggleDamageType::EquippedWeapon;
#pragma endregion

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	TObjectPtr<UFatedBrandAbilitySystemComponent> FatedBrandAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	TObjectPtr<UAttributeSet> FatedBrandAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	EStartUpCharacterName StartUpCharacterName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	TSoftObjectPtr<UDataAsset_StartUpDataBase> StartUpData;

	virtual void InitAbilityActorInfo() { }

	void AddCharacterAbilities() const;

	UPROPERTY(BlueprintReadWrite)
	bool bIsDeath = false;
};
