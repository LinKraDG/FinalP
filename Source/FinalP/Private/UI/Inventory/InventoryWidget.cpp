// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventoryWidget.h"

#include "Character/PlayerCharacter.h"
#include "Character/Components/InventoryComponent.h"
#include "Components/PanelWidget.h"
#include "Components/VerticalBox.h"
#include "UI/Inventory/UnicItem.h"

UPanelWidget* UInventoryWidget::GetItemsPanel()
{
	return itemsPanel;
}

void UInventoryWidget::ShowHideOptions(int id)
{
	UVerticalBox* options = nullptr;
	int count = 0;
	for (UWidget* itemWidget : itemsPanel->GetAllChildren())
	{

		options = Cast<UUnicItem>(itemsPanel->GetChildAt(count))->GetOptions();
		
		if (count == id)
		{
			if (options->IsVisible())
			{
				options->SetVisibility(ESlateVisibility::Hidden);
			}
			else
			{
				options->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else
		{
			options->SetVisibility(ESlateVisibility::Hidden);
		}
		
		count++;
	}
}

