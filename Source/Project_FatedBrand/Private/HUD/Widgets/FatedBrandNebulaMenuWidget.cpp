// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Widgets/FatedBrandNebulaMenuWidget.h"

#include "HUD/Widgets/FatedBrandNebulaSocketWidget.h"
#include "HUD/Widgets/FatedBrandHotBarWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Project_FatedBrand/Project_FatedBrand.h"

void UFatedBrandNebulaMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

    SlotWidgets.Empty();
    HotBarWidgets.Empty();

    for (int32 y = 1; y <= 4; ++y)
    {
        for (int32 x = 1; x <= 10; ++x)
        {
            const FString Name = FString::Printf(TEXT("Nebula_%dX%d"), y, x);
            if (UFatedBrandNebulaSocketWidget* Found = Cast<UFatedBrandNebulaSocketWidget>(GetWidgetFromName(*Name)))
            {
                SlotWidgets.Add(Found);
                Found->GetSelectImage()->SetVisibility(ESlateVisibility::Hidden);
            }
        }
    }

    if (SlotWidgets.IsValidIndex(0))
        SlotWidgets[0]->GetSelectImage()->SetVisibility(ESlateVisibility::Visible);

    for (int32 x = 1; x <= 6; ++x)
    {
        const FString Name = FString::Printf(TEXT("Nebula_HotBar_1X%d"), x);
        if (UFatedBrandHotBarWidget* Found = Cast<UFatedBrandHotBarWidget>(GetWidgetFromName(*Name)))
        {
            HotBarWidgets.Add(Found);
            Found->GetSelectImage()->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

void UFatedBrandNebulaMenuWidget::CallSelectSocket(int32 PrevIndex, int32 CurrentIndex)
{
    if (SlotWidgets.IsValidIndex(PrevIndex))
        SlotWidgets[PrevIndex]->GetSelectImage()->SetVisibility(ESlateVisibility::Hidden);

    if (SlotWidgets.IsValidIndex(CurrentIndex))
    {
	    SlotWidgets[CurrentIndex]->GetSelectImage()->SetVisibility(ESlateVisibility::Visible);
        SlotWidgets[CurrentIndex]->OnSelectSocketDelegate.Broadcast();
    }
}

void UFatedBrandNebulaMenuWidget::CallSelectSocketFocusing(const int32 CurrentIndex, const bool IsSelectSocketFocusing)
{
    if (IsSelectSocketFocusing)
    {
		for (UFatedBrandNebulaSocketWidget* SlotWidget : SlotWidgets)
	    {
    		SlotWidget->SetRenderOpacity(0.5f);
	    }
	    if (SlotWidgets.IsValidIndex(CurrentIndex) && IsSelectSocketFocusing) SlotWidgets[CurrentIndex]->SetRenderOpacity(1.f);
        if (HotBarWidgets.IsValidIndex(0)) HotBarWidgets[0]->GetSelectImage()->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
    	for (UFatedBrandNebulaSocketWidget* SlotWidget : SlotWidgets)
	    {
    		SlotWidget->SetRenderOpacity(1.f);
	    }

		for (const UFatedBrandHotBarWidget* SlotWidget : HotBarWidgets)
		{
			SlotWidget->GetSelectImage()->SetVisibility(ESlateVisibility::Hidden); 
		}
    }
}

void UFatedBrandNebulaMenuWidget::CallSelectHotBar(const int32 PrevIndex, const int32 CurrentIndex)
{
    if (HotBarWidgets.IsValidIndex(PrevIndex))
        HotBarWidgets[PrevIndex]->GetSelectImage()->SetVisibility(ESlateVisibility::Hidden);

    if (HotBarWidgets.IsValidIndex(CurrentIndex))
    {
	    HotBarWidgets[CurrentIndex]->GetSelectImage()->SetVisibility(ESlateVisibility::Visible);
        HotBarWidgets[CurrentIndex]->CurrentHotBarDelegate.Broadcast();
    }
}

void UFatedBrandNebulaMenuWidget::SetNebulaDescription(const FString InDescription)
{
    if (!NebulaDescription.IsValid()) return;
    
	NebulaDescription->SetText(FText::FromString(InDescription));
}

void UFatedBrandNebulaMenuWidget::CallSelectSocketConfirm(FFatedBrandAbilityInfo CurrentInfo, const int32 CurrentIndex)
{
	for (UFatedBrandNebulaSocketWidget* SlotWidget : SlotWidgets)
    {
    	SlotWidget->SetRenderOpacity(1.f);
    }

	for (const UFatedBrandHotBarWidget* SlotWidget : HotBarWidgets)
	{
		SlotWidget->GetSelectImage()->SetVisibility(ESlateVisibility::Hidden);
        HotBarWidgets[CurrentIndex]->SetWidget(CurrentInfo);
	}
}
