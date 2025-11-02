// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FatedBrandStructTypes.h"
#include "GameFramework/Actor.h"
#include "FatedBrandProjectileActor.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class PROJECT_FATEDBRAND_API AFatedBrandProjectileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AFatedBrandProjectileActor();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	FDamageEffectParams DamageEffectParams;

	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;

	bool IsValidOverlap(AActor* OtherActor) const;

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	UFUNCTION(BlueprintCallable)
	virtual void OnHit();

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool bHit = false;

private :
	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> Sphere;
};
