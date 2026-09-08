// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/ContainerInventory/UnicContainerWidget.h"

#include "Character/PlayerCharacter.h"
#include "Character/Components/InventoryComponent.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UI/PlayerHUD.h"

void UUnicContainerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	itemButton->OnPressed.AddUniqueDynamic(this, &UUnicContainerWidget::OnItemButtonPressed);

	selectedBorder->SetVisibility(ESlateVisibility::Hidden);
}

UBorder* UUnicContainerWidget::GetFrame()
{
	return frame;
}

UBorder* UUnicContainerWidget::GetItem()
{
	return item;
}

bool UUnicContainerWidget::GetSelected()
{
	return selected;
}

int UUnicContainerWidget::GetItemID()
{
	return itemData->item_ID;
}

int UUnicContainerWidget::GetQuantity()
{
	return quantity;
}


void UUnicContainerWidget::SetNoneImage()
{
	//frame->SetVisibility(ESlateVisibility::Visible);
	//frame->SetBrushFromTexture(imageFrame);
}

void UUnicContainerWidget::SetFrameImage()
{
	frame->SetVisibility(ESlateVisibility::Visible);
}

void UUnicContainerWidget::SetData(int slot_ID, FItemData& data, int itemQuantity)
{
	itemData = &data;
	quantity = itemQuantity;
	ID = slot_ID;
	frame->SetVisibility(ESlateVisibility::Visible);
	item->SetBrushFromTexture(data.image);
	quantityText->SetText(FText::AsNumber(itemQuantity));
	frame->SetBrushFromTexture(data.image);
	item->SetVisibility(ESlateVisibility::Visible);
}

void UUnicContainerWidget::SetEmptyData(int slot_ID)
{
	ID = slot_ID;
	item->SetVisibility(ESlateVisibility::Hidden);
}

void UUnicContainerWidget::OnItemButtonPressed()
{
	selected = !selected;
	switch (selected)
	{
		case true:
			selectedBorder->SetVisibility(ESlateVisibility::Visible);
			break;
		case false:
			selectedBorder->SetVisibility(ESlateVisibility::Hidden);
			break;
	}
}
