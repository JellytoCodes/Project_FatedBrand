// Fill out your copyright notice in the Description page of Project Settings.

#include "FatedBrandGameplayTags.h"

namespace FatedBrandGameplayTags
{
#pragma region Input
	/** ~Begin Native Input */
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Input_Move, "Input.Move")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Input_Jump, "Input.Jump")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Input_NebulaMenu, "Input.NebulaMenu")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Input_WidgetSelect, "Input.WidgetSelect")
	/** ~End Native Input */

	/** ~Begin Ability Input */
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Input_Key_SpaceBar, "Input.Key.SpaceBar")

	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Input_Key_Q, "Input.Key.Q")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Input_Key_W, "Input.Key.W")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Input_Key_E, "Input.Key.E")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Input_Key_1, "Input.Key.1")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Input_Key_2, "Input.Key.2")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Input_Key_3, "Input.Key.3")

	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Input_Block, "Input.Block")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Input_VitalSurge, "Input.VitalSurge")
	/** ~End Ability Input */
#pragma endregion
	
#pragma region Abilities Status
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Abilities_None, "Abilities.None")

	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Abilities_Status_Equipped, "Abilities.Status.Equipped")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Abilities_Status_Eligible, "Abilities.Status.Eligible")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Abilities_Status_Unlocked, "Abilities.Status.Unlocked")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Abilities_Status_Locked, "Abilities.Status.Locked")
#pragma endregion

#pragma region Abilities Offensive
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Abilities_Offensive_Type, "Abilities.Offensive.Type")
#pragma endregion

#pragma region Abilities Passive
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Abilities_Passive_Type, "Abilities.Passive.Type")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Abilities_Passive_DoubleJump, "Abilities.Passive.DoubleJump")
#pragma endregion

#pragma region Damage
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Damage_Status_Attack, "Damage.Status.Attack")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Damage_Status_Blocking, "Damage.Status.Blocking")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Damage_MeleeAttack, "Damage.MeleeAttack")

#pragma endregion

#pragma region Event
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Event_SuccessfulBlock, "Event.SuccessfulBlock")
	
#pragma endregion
}
