// Fill out your copyright notice in the Description page of Project Settings.

#include "FatedBrandGameplayTags.h"

namespace FatedBrandGameplayTags
{
#pragma region Input
	/** ~Begin Native Input */
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Input_Move, "Input.Move")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Input_Jump, "Input.Jump")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Input_NebulaMenu, "Input.NebulaMenu")
	/** ~End Native Input */

	/** ~Begin Ability Input */
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Input_Key_SpaceBar, "Input.Key.SpaceBar")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Input_Key_Q, "Input.Key.Q")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Input_Key_W, "Input.Key.W")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Input_Key_E, "Input.Key.E")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Input_Key_1, "Input.Key.1")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Input_Key_2, "Input.Key.2")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Input_Key_3, "Input.Key.3")
	/** ~End Ability Input */
#pragma endregion

	
#pragma region Abilities Status
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Abilities_None, "Abilities.None")

	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Abilities_Status_Equipped, "Abilities.Status.Equipped")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Abilities_Status_Eligible, "Abilities.Status.Eligible")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Abilities_Status_Unlocked, "Abilities.Status.Unlocked")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Abilities_Status_Locked, "Abilities.Status.Locked")
#pragma endregion

#pragma region Abilities Active

#pragma endregion

#pragma region Abilities Passive
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Abilities_Passive_DoubleJump, "Abilities.Passive.DoubleJump")
#pragma endregion
}
