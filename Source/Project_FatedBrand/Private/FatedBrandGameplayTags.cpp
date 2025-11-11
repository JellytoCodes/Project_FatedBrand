// Fill out your copyright notice in the Description page of Project Settings.

#include "FatedBrandGameplayTags.h"

namespace FatedBrandGameplayTags
{
#pragma region Input
	/** ~Begin Native Input */
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Input_Move, "Input.Move")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Input_Jump, "Input.Jump")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Input_NebulaMenu, "Input.NebulaMenu")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Input_PauseMenu, "Input.PauseMenu")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Input_WidgetSelect, "Input.WidgetSelect")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Input_WidgetDeSelect, "Input.WidgetDeSelect")
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
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Abilities_Status_Unlocked, "Abilities.Status.Unlocked")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Abilities_Status_Locked, "Abilities.Status.Locked")
#pragma endregion

#pragma region Ability Activate
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Ability_Activate_MeleeAttack, "Abilities.Activate.MeleeAttack")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Ability_Activate_BlastingZone, "Abilities.Activate.BlastingZone")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Ability_Activate_Blocking, "Abilities.Activate.Blocking")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Ability_Activate_VitalSurge, "Abilities.Activate.VitalSurge")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Ability_Activate_JugularRip, "Abilities.Activate.JugularRip")
#pragma endregion

#pragma region Abilities Offensive
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Abilities_Offensive_Type, "Abilities.Offensive.Type")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Abilities_Offensive_MeleeAttack, "Abilities.Offensive.MeleeAttack")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Abilities_Offensive_NobleBlood, "Abilities.Offensive.NobleBlood")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Abilities_Offensive_BlastingZone, "Abilities.Offensive.BlastingZone")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Abilities_Offensive_Blocking, "Abilities.Offensive.Blocking")
#pragma endregion

#pragma region Abilities Offecsive Cooldown
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Cooldown_BlastingZone, "Cooldown.BlastingZone")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Cooldown_NobleBlood, "Cooldown.NobleBlood")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Cooldown_LivingDead, "Cooldown.LivingDead")
#pragma endregion

#pragma region Abilities Passive
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Abilities_Passive_Type, "Abilities.Passive.Type")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Abilities_Passive_DoubleJump, "Abilities.Passive.DoubleJump")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Abilities_Passive_LivingDead, "Abilities.Passive.LivingDead")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Abilities_Passive_JugularRip, "Abilities.Passive.JugularRip")
#pragma endregion

#pragma region Damage
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Damage_Type_Physical, "Damage.Type.Physical")

#pragma endregion

#pragma region AttackType
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Attack_Type_Melee, "Attack.Type.Melee")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Attack_Type_Range, "Attack.Type.Range")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Attack_Type_JumpAttack, "Attack.Type.JumpAttack")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Attack_Type_RangedAttack_A, "Attack.Type.RangedAttack.A")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Attack_Type_RangedAttack_B, "Attack.Type.RangedAttack.B")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Attack_Type_RangedAttack_C, "Attack.Type.RangedAttack.C")
#pragma endregion
	
#pragma region Event
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Event_SuccessfulBlock, "Event.SuccessfulBlock")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Event_HitReact, "Event.HitReact")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Event_WakeUp, "Event.WakeUp")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Event_Montage_Range, "Event.Montage.Range")
	UE_DEFINE_GAMEPLAY_TAG(FatedBrandGameplayTags::Event_Montage_JumpAttack, "Event.Montage.JumpAttack")
#pragma endregion
}

