// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace FatedBrandGameplayTags
{
#pragma region Input
	/** ~Begin Native Input */
	PROJECT_FATEDBRAND_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Move)
	PROJECT_FATEDBRAND_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Jump)
	PROJECT_FATEDBRAND_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_NebulaMenu)
	/** ~End Native Input */

	/** ~Begin Ability Input */
	PROJECT_FATEDBRAND_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Key_SpaceBar)
	PROJECT_FATEDBRAND_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Key_Q)
	PROJECT_FATEDBRAND_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Key_W)
	PROJECT_FATEDBRAND_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Key_E)
	PROJECT_FATEDBRAND_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Key_1)
	PROJECT_FATEDBRAND_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Key_2)
	PROJECT_FATEDBRAND_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Key_3)
	/** ~End Ability Input */
#pragma endregion

	
#pragma region Abilities Status
	PROJECT_FATEDBRAND_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_None)

	PROJECT_FATEDBRAND_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Status_Equipped)
	PROJECT_FATEDBRAND_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Status_Eligible)
	PROJECT_FATEDBRAND_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Status_Unlocked)
	PROJECT_FATEDBRAND_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Status_Locked)
#pragma endregion

#pragma region Abilities Active

#pragma endregion

#pragma region Abilities Passive
	PROJECT_FATEDBRAND_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Passive_DoubleJump)
#pragma endregion
}