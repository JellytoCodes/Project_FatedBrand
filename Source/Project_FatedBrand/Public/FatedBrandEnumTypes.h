// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FatedBrandEnumTypes.generated.h"

UENUM(BlueprintType)
enum ENebulaSelectSocket : uint8
{
	NebulaSocket1,
	NebulaSocket2,
	NebulaSocket3,
	NebulaSocket4,
	NebulaSocket5,
	NebulaSocket6,
	NebulaSocket7,
	NebulaSocket8,
	NebulaSocket9,
	NebulaSocket10,
	NebulaSocket11,
	NebulaSocket12,
	NebulaSocket13,
	NebulaSocket14,
	NebulaSocket15,
	NebulaSocket16,
	NebulaSocket17,
	NebulaSocket18,
	NebulaSocket19,
	NebulaSocket20,
	NebulaSocket21,
	NebulaSocket22,
	NebulaSocket23,
	NebulaSocket24,
	NebulaSocket25,
	NebulaSocket26,
	NebulaSocket27,
	NebulaSocket28,
	NebulaSocket29,
	NebulaSocket30,
	NebulaSocket31,
	NebulaSocket32,
	NebulaSocket33,
	NebulaSocket34,
	NebulaSocket35,
	NebulaSocket36,
	NebulaSocket37,
	NebulaSocket38,
	NebulaSocket39,
	NebulaSocket40
};


UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	RemoveOnEndOverlap,
	DoNotRemove
};

UENUM()
enum class EWidgetOpenState : uint8
{
	None,
	NebulaMenu,
	PauseMenu,
	KeyDescription,
	AttributeMenu,
};