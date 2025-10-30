// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/Widgets/FatedBrandUserWidget.h"

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
            if (UFatedBrandUserWidget* Found = Cast<UFatedBrandUserWidget>(GetWidgetFromName(*Name)))
            {
                SlotWidgets.Add(Found);
                Found->SetWidgetController(InWidgetController);
            }
        }
    }

    if (SlotWidgets.IsValidIndex(0))
    {
        UImage* FirstSelectImage = Cast<UImage>(SlotWidgets[0]->GetWidgetFromName(TEXT("Select_Image")));
		FirstSelectImage->SetVisibility(ESlateVisibility::Visible);    
    }
}

void UFatedBrandUserWidget::CallSelectNebulaSocket(int32 PrevIndex, int32 CurrentIndex)
{
    if (SlotWidgets.IsValidIndex(PrevIndex))
    {
	    UImage* PrevSelectImage = Cast<UImage>(SlotWidgets[PrevIndex]->GetWidgetFromName(TEXT("Select_Image")));
        PrevSelectImage->SetVisibility(ESlateVisibility::Hidden);
    }

    if (SlotWidgets.IsValidIndex(CurrentIndex))
    {
	    UImage* CurrentSelectImage = Cast<UImage>(SlotWidgets[CurrentIndex]->GetWidgetFromName(TEXT("Select_Image")));
        CurrentSelectImage->SetVisibility(ESlateVisibility::Visible);
    }

}

void UFatedBrandUserWidget::CallSelectSocketFocusing(const int32 CurrentIndex, const bool IsSelectSocketFocusing)
{
    if (IsSelectSocketFocusing)
    {
		for (UFatedBrandUserWidget* SlotWidget : SlotWidgets)
	    {
            if (SlotWidgets[CurrentIndex] == SlotWidget) continue;
    		SlotWidget->SetRenderOpacity(0.5f);
	    }
        SlotWidgets[CurrentIndex]->OnSelectSocketDelegate.Broadcast();
    }
    else
    {
    	for (UFatedBrandUserWidget* SlotWidget : SlotWidgets)
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
        if (UFatedBrandUserWidget* Found = Cast<UFatedBrandUserWidget>(GetWidgetFromName(*Name)))
        {
            QuickSlots.Add(Found);
            Found->SetWidgetController(InWidgetController);
        }
    }
}

void UFatedBrandUserWidget::CallSelectQuickSlot(int32 PrevIndex, int32 CurrentIndex)
{
    if (QuickSlots.IsValidIndex(PrevIndex))
    {
    	UImage* PrevSelectImage = Cast<UImage>(QuickSlots[PrevIndex]->GetWidgetFromName(TEXT("Select_Image")));
        PrevSelectImage->SetVisibility(ESlateVisibility::Hidden);
    }

    if (QuickSlots.IsValidIndex(CurrentIndex))
    {
    	UImage* CurrentSelectImage = Cast<UImage>(QuickSlots[CurrentIndex]->GetWidgetFromName(TEXT("Select_Image")));
        CurrentSelectImage->SetVisibility(ESlateVisibility::Visible);
        QuickSlots[CurrentIndex]->SelectQuickSlotDelegate.Broadcast();
    }
}

void UFatedBrandUserWidget::CallDeselectQuickSlot()
{
	for (const UFatedBrandUserWidget* SlotWidget : QuickSlots)
	{
		UImage* SelectImage = Cast<UImage>(SlotWidget->GetWidgetFromName(TEXT("Select_Image")));
		SelectImage->SetVisibility(ESlateVisibility::Hidden);
	}
}