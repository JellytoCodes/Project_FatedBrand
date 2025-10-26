// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/Widgets/FatedBrandUserWidget.h"

#include "FatedBrandGameplayTags.h"
#include "HUD/Widgets/FatedBrandNebulaSocketWidget.h"
#include "HUD/Widgets/FatedBrandHotBarWidget.h"
#include "Components/Image.h"

void UFatedBrandUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}

void UFatedBrandUserWidget::InitializeNebulaSocket(UObject* InWidgetController)
{
    SlotWidgets.Empty();

    for (int32 y = 1; y <= 4; ++y)
    {
        for (int32 x = 1; x <= 10; ++x)
        {
            const FString Name = FString::Printf(TEXT("Nebula_%dX%d"), y, x);
            if (UFatedBrandNebulaSocketWidget* Found = Cast<UFatedBrandNebulaSocketWidget>(GetWidgetFromName(*Name)))
            {
                SlotWidgets.Add(Found);
                Found->GetSelectImage()->SetVisibility(ESlateVisibility::Hidden);
                Found->SetWidgetController(InWidgetController);
            }
        }
    }

    if (SlotWidgets.IsValidIndex(0))
    SlotWidgets[0]->GetSelectImage()->SetVisibility(ESlateVisibility::Visible);
}

void UFatedBrandUserWidget::CallSelectNebulaSocket(int32 PrevIndex, int32 CurrentIndex)
{
    if (SlotWidgets.IsValidIndex(PrevIndex))
        SlotWidgets[PrevIndex]->GetSelectImage()->SetVisibility(ESlateVisibility::Hidden);

    if (SlotWidgets.IsValidIndex(CurrentIndex))
    {
	    SlotWidgets[CurrentIndex]->GetSelectImage()->SetVisibility(ESlateVisibility::Visible);
        SlotWidgets[CurrentIndex]->OnSelectSocketDelegate.Broadcast();
    }
}

void UFatedBrandUserWidget::CallSelectSocketFocusing(const int32 CurrentIndex, const bool IsSelectSocketFocusing)
{
    if (IsSelectSocketFocusing)
    {
		for (UFatedBrandNebulaSocketWidget* SlotWidget : SlotWidgets)
	    {
            if (SlotWidgets[CurrentIndex] == SlotWidget) continue;
    		SlotWidget->SetRenderOpacity(0.5f);
	    }
        if (QuickSlots.IsValidIndex(0)) QuickSlots[0]->GetSelectImage()->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
    	for (UFatedBrandNebulaSocketWidget* SlotWidget : SlotWidgets)
	    {
    		SlotWidget->SetRenderOpacity(1.f);
	    }
    }
}

void UFatedBrandUserWidget::InitializeQuickSlot(UObject* InWidgetController)
{
    QuickSlots.Empty();

    for (int32 x = 1; x <= 6; ++x)
    {
        const FString Name = FString::Printf(TEXT("Nebula_HotBar_1X%d"), x);
        if (UFatedBrandHotBarWidget* Found = Cast<UFatedBrandHotBarWidget>(GetWidgetFromName(*Name)))
        {
            QuickSlots.Add(Found);
            Found->GetSelectImage()->SetVisibility(ESlateVisibility::Hidden);
            Found->SetWidgetController(InWidgetController);
        }
    }
}

void UFatedBrandUserWidget::CallSelectQuickSlot(int32 PrevIndex, int32 CurrentIndex)
{
    if (QuickSlots.IsValidIndex(PrevIndex)) QuickSlots[PrevIndex]->GetSelectImage()->SetVisibility(ESlateVisibility::Hidden);

    if (QuickSlots.IsValidIndex(CurrentIndex))
    {
	    QuickSlots[CurrentIndex]->GetSelectImage()->SetVisibility(ESlateVisibility::Visible);
        QuickSlots[CurrentIndex]->CurrentHotBarDelegate.Broadcast();
    }
}

void UFatedBrandUserWidget::CallDeselectQuickSlot(const bool IsSelectSocketFocusing)
{
	for (const UFatedBrandHotBarWidget* SlotWidget : QuickSlots)
	{
		SlotWidget->GetSelectImage()->SetVisibility(ESlateVisibility::Hidden);
	}
}