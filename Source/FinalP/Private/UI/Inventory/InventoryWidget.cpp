// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventoryWidget.h"

#include "Character/PlayerCharacter.h"
#include "Character/Components/InventoryComponent.h"
#include "Components/PanelWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/VerticalBox.h"
#include "Enums/StructuresClassify.h"
#include "Structs/ConstructionData.h"
#include "UI/Inventory/UnicItem.h"

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

void UInventoryWidget::ItemSlotCreation()
{
	APlayerCharacter* player = Cast<APlayerCharacter>(GetOwningPlayer()->GetCharacter());
	if (!IsValid(player)) return;

	int inventorySize = player->GetInventory()->GetSize();

	UDataTable* itemsDataTable = player->GetInventory()->GetItemDataTable();
	if (!IsValid(itemsDataTable)) return;

	TMap<int, FInventoryItem> playerItemsInventory = player->GetInventory()->GetInventoryData();

	//crear los slots

	for (int i = 0; i < inventorySize; i++)
	{
		if (playerItemsInventory[i].ItemID==-1) CreateEmptyItemSlot(i);
		else CreateItemSlot(i, itemsDataTable, playerItemsInventory[i].ItemID, playerItemsInventory[i].Amount);
	}
	
}

void UInventoryWidget::CreateItemSlot(int slotID, UDataTable* itemsDataTable, int itemID, int itemQuantity)
{
	//FConstructionData* dataRow = dataTable->FindRow<FConstructionData>(row,"");

	UUnicItem* newItemSlot = CreateWidget<UUnicItem>(GetWorld(), itemsWidget);

	FName findedRowName = NAME_None;
	itemsDataTable->ForeachRow<FItemData>(TEXT("Context"), [&](const FName& RowName, const FItemData& RowData)
	{
		if (RowData.item_ID == itemID)
		{
			findedRowName = RowName;
			return;
		}
	});
	
	FItemData* row = itemsDataTable->FindRow<FItemData>(findedRowName, "");
	
	newItemSlot->SetData(slotID, *row, itemQuantity);
	Cast<UUniformGridPanel>(itemsPanel)->AddChildToUniformGrid(newItemSlot, slotID/6, slotID%6);
}

void UInventoryWidget::CreateEmptyItemSlot(int slotID)
{
	UUnicItem* newItemSlot = CreateWidget<UUnicItem>(GetWorld(), itemsWidget);
	
	newItemSlot->SetEmptyData(slotID);
	Cast<UUniformGridPanel>(itemsPanel)->AddChildToUniformGrid(newItemSlot, slotID/6, slotID%6);
}

void UInventoryWidget::RemoveSlots()
{
	Cast<UUniformGridPanel>(itemsPanel)->ClearChildren();
}

