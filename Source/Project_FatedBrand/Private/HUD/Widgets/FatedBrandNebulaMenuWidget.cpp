// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Widgets/FatedBrandNebulaMenuWidget.h"

#include "Components/TextBlock.h"

void UFatedBrandNebulaMenuWidget::SetNebulaDescription(const FString InDescription)
{
    if (!NebulaDescription.IsValid()) return;
    
	NebulaDescription->SetText(FText::FromString(InDescription));
}