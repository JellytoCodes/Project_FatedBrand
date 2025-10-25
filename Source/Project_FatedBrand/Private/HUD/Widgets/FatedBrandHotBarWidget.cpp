// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Widgets/FatedBrandHotBarWidget.h"

#include "Components/Image.h"
#include "Project_FatedBrand/Project_FatedBrand.h"

void UFatedBrandHotBarWidget::SetWidget(FFatedBrandAbilityInfo AbilityInfo)
{
	SlotAbilityInfo = AbilityInfo;

	Debug::Print(SlotAbilityInfo.AbilityTag.ToString());
}
