// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/ContainerInventory/ContainerWidget.h"

#include "Character/PlayerCharacter.h"
#include "Character/Components/InventoryComponent.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "Construction/Child/OrganizationConstruct.h"
#include "UI/Inventory/UnicItem.h"
#include "UI/Inventory/ContainerInventory/UnicContainerWidget.h"

void UContainerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	playerToContainerButton->OnPressed.AddUniqueDynamic(this, &UContainerWidget::OnPToCButtonPressed);
	containerToPlayerButton->OnPressed.AddUniqueDynamic(this, &UContainerWidget::OnCToPButtonPressed);
}

void UContainerWidget::ItemSlotCreation(APlayerCharacter* playerI, AOrganizationConstruct* containerI)
{
	if (!IsValid(playerI)||!IsValid(containerI)) return;

	player = playerI;
	container = containerI;

	int playerInventorySize = player->GetInventory()->GetSize();
	int containerInventorySize = container->GetInventory()->GetSize();
	
	if (!IsValid(itemsDataTable)) return;

	TMap<int, FInventoryItem> playerItemsInventory = player->GetInventory()->GetInventoryData();
	TMap<int, FInventoryItem> containerItemsInventory = container->GetInventory()->GetInventoryData();

	//crear los slots
	//jugador
	for (int i = 0; i < playerInventorySize; i++)
	{
		if (playerItemsInventory[i].ItemID==-1) CreatePlayerEmptyItemSlot(i);
		else CreatePlayerItemSlot(i, playerItemsInventory[i].ItemID, playerItemsInventory[i].Amount);
	}
	
	//contenedor
	for (int i = 0; i < containerInventorySize; i++)
	{
		if (containerItemsInventory[i].ItemID==-1) CreateContainerEmptyItemSlot(i);
		else CreateContainerItemSlot(i, containerItemsInventory[i].ItemID, containerItemsInventory[i].Amount);
	}
}

void UContainerWidget::CreatePlayerItemSlot(int slotID, int itemID, int itemQuantity)
{
	UUnicContainerWidget* newItemSlot = CreateWidget<UUnicContainerWidget>(GetWorld(), itemsWidget);

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
	Cast<UUniformGridPanel>(playerItemsPanel)->AddChildToUniformGrid(newItemSlot, slotID/6, slotID%6);
}

void UContainerWidget::CreateContainerItemSlot(int slotID, int itemID, int itemQuantity)
{
	UUnicContainerWidget* newItemSlot = CreateWidget<UUnicContainerWidget>(GetWorld(), itemsWidget);

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
	Cast<UUniformGridPanel>(containerItemsPanel)->AddChildToUniformGrid(newItemSlot, slotID/6, slotID%6);
}

void UContainerWidget::CreatePlayerEmptyItemSlot(int slotID)
{
	UUnicContainerWidget* newItemSlot = CreateWidget<UUnicContainerWidget>(GetWorld(), itemsWidget);
	
	newItemSlot->SetEmptyData(slotID);
	Cast<UUniformGridPanel>(playerItemsPanel)->AddChildToUniformGrid(newItemSlot, slotID/6, slotID%6);
}

void UContainerWidget::CreateContainerEmptyItemSlot(int slotID)
{
	UUnicContainerWidget* newItemSlot = CreateWidget<UUnicContainerWidget>(GetWorld(), itemsWidget);
	
	newItemSlot->SetEmptyData(slotID);
	Cast<UUniformGridPanel>(containerItemsPanel)->AddChildToUniformGrid(newItemSlot, slotID/6, slotID%6);
}

void UContainerWidget::RemoveSlots()
{
	Cast<UUniformGridPanel>(playerItemsPanel)->ClearChildren();
	Cast<UUniformGridPanel>(containerItemsPanel)->ClearChildren();
}

void UContainerWidget::OnPToCButtonPressed()
{
	UInventoryComponent* playerInventory = player->GetInventory();
	if (!IsValid(playerInventory)) return;
	UInventoryComponent* containerInventory = container->GetInventory();
	if (!IsValid(containerInventory)) return;

	for (UWidget* i : playerItemsPanel->GetAllChildren())
	{
		UUnicContainerWidget* item = Cast<UUnicContainerWidget>(i);
		if (!IsValid(item)) return;
		if (!item->GetSelected()) continue;

		containerInventory->LoadItem(item->GetItemID(), item->GetQuantity());
		playerInventory->RemoveItem(item->GetItemID(), item->GetQuantity());
	}
	RePrintInventories();
}

void UContainerWidget::OnCToPButtonPressed()
{
	UInventoryComponent* playerInventory = player->GetInventory();
	if (!IsValid(playerInventory)) return;
	UInventoryComponent* containerInventory = container->GetInventory();
	if (!IsValid(containerInventory)) return;

	for (UWidget* i : containerItemsPanel->GetAllChildren())
	{
		UUnicContainerWidget* item = Cast<UUnicContainerWidget>(i);
		if (!IsValid(item)) return;
		if (!item->GetSelected()) continue;

		playerInventory->LoadItem(item->GetItemID(), item->GetQuantity());
		containerInventory->RemoveItem(item->GetItemID(), item->GetQuantity());
	}
	RePrintInventories();
}

void UContainerWidget::RePrintInventories()
{
	RemoveSlots();
	ItemSlotCreation(player, container);
}
